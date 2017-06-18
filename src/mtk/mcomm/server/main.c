#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>

#include "../types.h"
#include "../vector.h"

static bool running;
static int listenfd;

void create_detached(void* (*entrypoint)(void*), void* arg) {
  pthread_t thread;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_create(&thread, &attr, entrypoint, arg); 
}

void* thread_connect(void* arg) {
  int connfd = *(int*) arg;

  time_t ticks = time(NULL);
  char sendBuff[1024];

  snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
  write(connfd, sendBuff, strlen(sendBuff)); 

  close(connfd);

  pthread_exit(NULL);
}

void* thread_listen(void* arg) {
  struct sockaddr_in serv_addr; 
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  int port = *(int*) arg;

  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port); 

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

  listen(listenfd, 10); 

  while(running)
  {
    int connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

    create_detached(&thread_connect, (void*) &connfd);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  int port = 5000;
  char* pass = NULL;

  if (argc == 3) {
    sscanf(argv[2], "%d", &port);
    pass = argv[1];
  } else if (argc == 2) { 
    pass = argv[1];
  } else if (argc != 1) {
    printf("Usage: %s [pass] [port]\n", argv[0]);
    exit(1);
  }

  running = true;

  pthread_t listener;
  pthread_create(&listener, NULL, &thread_listen, (void*) &port);

  // ...
  getc(stdin);

  running = false;
  shutdown(listenfd, SHUT_RDWR);
  close(listenfd);

  pthread_join(listener, NULL);

  pthread_exit(NULL);
  // return 0;
}
