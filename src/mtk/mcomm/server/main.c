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
#include "../concurr_vector.h"
#include "../protocol.h"
#include "../net.h"

#include "log.h"

typedef struct {
  name_t name;
  int fd;
} connection_t;

static bool running;
static int listenfd;
static char* password;
static cvector_t users;

void create_detached(void* (*entrypoint)(void*), void* arg) {
  pthread_t thread;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_create(&thread, &attr, entrypoint, arg); 
}

void send_packet(void* conn, void* packet) {
  connection_t* connection = (connection_t*) conn;

  write(connection->fd, packet, sizeof(packet_t));
}

void broadcast(packet_t* packet) {
  cvec_foreach(&users, &send_packet, packet);
}

void* thread_receive(void* arg) {
  connection_t conn;
  packet_t packet;
  
  conn.fd = *(int*) arg;

  if (password != NULL) {
    packet.type = REQUEST_PASSWORD;
    write(conn.fd, &packet, sizeof(packet_t));

    if (packet.type != RESPONSE_PASSWORD)
      pthread_exit(NULL);

    read_exact(conn.fd, (char*) &packet, sizeof(packet_t));
    
    if (strcmp(packet.data.resppass.pass, password) != 0)
      pthread_exit(NULL);
  }

  packet.type = REQUEST_NAME;
  write(conn.fd, &packet, sizeof(packet_t));
  read_exact(conn.fd, (char*) &packet, sizeof(packet_t));

  if (packet.type != RESPONSE_NAME)
    pthread_exit(NULL);

  conn.name = packet.data.respname.name;
  cvec_add(&users, &conn);

  logger("%s connected successfuly", conn.name.value);

  while (running && read_exact(conn.fd, (char*) &packet, sizeof(packet_t)) > 0) {
    if (packet.type == REQUEST_MESSAGE) {
      logger("%s: %s", conn.name.value, packet.data.reqmessage.message);

      packet.type = RESPONSE_MESSAGE;
      packet.data.respmessage.author = conn.name;

      broadcast(&packet);
    }
  }

  cvec_fremove(&users, &conn);
  close(conn.fd);

  logger("%s disconnected", conn.name.value);

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

  logger("Server started on port: %d", port);

  while(running)
  {
    int connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

    if (connfd > 0)
      create_detached(&thread_receive, (void*) &connfd);
  }

  pthread_exit(NULL);
}

void log_user(void* conn, void* capture) {
  logger("  %s", ((connection_t*) conn)->name.value);
}

void loop_command() {
  char* command = NULL;
  size_t len = 0;

  while (running) {
    int n = getline(&command, &len, stdin);

    command[n - 1] = '\0';

    if (strcmp(command, "exit") == 0) {
      logger("Shutting down the server...");
      running = false;
    } else if (strcmp(command, "list") == 0) {
      logger("Currently online: ");
      cvec_foreach(&users, &log_user, NULL);
    }

    free(command);
    command = NULL;
  }
}

int main(int argc, char *argv[])
{
  int port = 5000;
  password = NULL;

  if (argc == 3) {
    sscanf(argv[1], "%d", &port);
    password = argv[2];
  } else if (argc == 2) { 
    sscanf(argv[1], "%d", &port);
  } else if (argc != 1) {
    printf("Usage: %s [port] [password]\n", argv[0]);
    exit(1);
  }

  logger("Starting server...");

  running = true;

  cvec_init(&users, sizeof(connection_t));

  pthread_t listener;
  pthread_create(&listener, NULL, &thread_listen, (void*) &port);

  loop_command(); 

  shutdown(listenfd, SHUT_RDWR);
  close(listenfd);

  cvec_free(&users);

  pthread_join(listener, NULL);

  pthread_exit(NULL);
}
