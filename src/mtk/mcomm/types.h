#ifndef TYPES_H
#define TYPES_H

typedef enum {
  false = (1 == 0),
  true = (1 == 1)
} bool;

typedef struct {
  char value[32];
} name_t;

#endif
