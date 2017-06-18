#ifndef LOG_H
#define LOG_H

int init_logger();

void free_logger();

void logger(const char* format, ...);

#endif
