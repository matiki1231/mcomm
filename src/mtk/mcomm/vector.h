#ifndef VECTOR_H
#define VECTOR_H

#include "types.h"

#include <stdlib.h>

#define VECTOR_INIT_CAPACITY 10
#define VECTOR_RESIZE_TRESHOLD 5
#define VECTOR_RESIZE_EXCESS 3

//#define VEC_INIT(vec, elem_t) (vec_init((vec), sizeof(elem_t)))
//#define VEC_ADD(vec, elem) (vec_add((vec), &(elem)))

typedef struct {
  size_t capacity;
  size_t size;
  size_t elem_size;
  char* data;
} vector_t;

bool vec_init(vector_t* vector, size_t elem_size);
bool vec_resize(vector_t* vector, size_t capacity);
bool vec_add(vector_t* vector, const void* elem);
void* vec_get(vector_t* vector, int index);
int vec_find(vector_t* vector, const void* elem);
bool vec_remove(vector_t* vector, int index);
void vec_free(vector_t* vector);

static inline bool vec_clear(vector_t* vector) {
  return vec_resize(vector, 0);
}

#endif
