#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <string.h>
#include <pthread.h>

#include "../types.h"
#include "../protocol.h"
#include "../net.h"

#include "tui.h"

void* thread_receive(void* arg) {
  int sockfd = *(int*) arg;

  packet_t packet;

  while (read_exact(sockfd, (char*) &packet, sizeof(packet_t)) > 0) {
    if (packet.type == RESPONSE_MESSAGE) {
      respmessage_t* msg = &packet.data.respmessage;

      tui_lock();
      wprintw(win_convers, "%s: ", msg->author.value);
      wprintw(win_convers, "%s\n", msg->message);
      wrefresh(win_convers);
      tui_unlock();
    } 
  }

  pthread_exit(NULL);
}

void loop_send(int sockfd) {
  int chars_count = 0;
  packet_t packet;
  memset(&packet, 0, sizeof(packet_t)); 
  reqmessage_t* msg = &packet.data.reqmessage;

  while (true) {
    tui_lock();

    int c = wgetch(win_input);

    tui_unlock();

    if (c != ERR) {
      if (c == 10) {
        msg->message[chars_count] = '\0';
        chars_count = 0;
        tui_lock();
        wclear(win_input);
        tui_unlock();

        if (strcmp(msg->message, "/exit") == 0)
          return;

        packet.type = REQUEST_MESSAGE;

        if (write(sockfd, &packet, sizeof(packet_t)) < 0)
          return;
      } else {
        msg->message[chars_count++] = (char) c;
      }
    }
  } 
}

bool authorize(int sockfd) {
  packet_t packet;
  
  read_exact(sockfd, (char*) &packet, sizeof(packet));

  if (packet.type != REQUEST_NAME)
    return false;

  printf("Enter username:\n");
  scanf("%s", packet.data.respname.name.value);

  packet.type = RESPONSE_NAME;
  write(sockfd, (char*) &packet, sizeof(packet));

  return true;
}

int main(int argc, char *argv[]) {
  int sockfd = 0;
  struct sockaddr_in serv_addr; 

  if (argc != 2) {
    printf("\n Usage: %s <ip of server> \n", argv[0]);
    return 1;
  } 

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Error : Could not create socket \n");
    return 1;
  } 

  memset(&serv_addr, '0', sizeof(serv_addr)); 
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(5000); 

  if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
    printf("\n inet_pton error occured\n");
    return 1;
  } 

  if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\n Error : Connect Failed \n");
    return 1;
  }

   
  if (authorize(sockfd)) {
    tui_init();
  
    pthread_t receive;

    pthread_create(&receive, NULL, &thread_receive, &sockfd);

    loop_send(sockfd);

    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);

    pthread_join(receive, NULL);

    tui_free();
  }

  return 0;
}
