#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

static pthread_mutex_t logmut;

int init_logger() {
  return pthread_mutex_init(&logmut, NULL);
}

void free_logger() {
  pthread_mutex_destroy(&logmut);
}

void logger(const char* format, ...) {
  pthread_mutex_lock(&logmut);
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  printf("\n");
  va_end(args);
  pthread_mutex_unlock(&logmut);
}
