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

      wprintw(win_convers, "%s: ", msg->author.value);
      wprintw(win_convers, "%s\n", msg->message);
      wrefresh(win_convers);
    } 
  }

  pthread_exit(NULL);
}

void loop_send(int sockfd) {
  packet_t packet;
  memset(&packet, 0, sizeof(packet_t)); 

  while (true) {
    reqmessage_t* msg = &packet.data.reqmessage;

    wgetstr(win_input, msg->message);
    wclear(win_input);

    if (strcmp(msg->message, "/exit") == 0)
      return;

    packet.type = REQUEST_MESSAGE;

    if (write(sockfd, &packet, sizeof(packet_t)) < 0)
      return;

  } 
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

  tui_init();
  
  pthread_t receive;

  pthread_create(&receive, NULL, &thread_receive, &sockfd);

  loop_send(sockfd);

  pthread_join(receive, NULL);

  tui_free();

  close(sockfd);

  return 0;
}
