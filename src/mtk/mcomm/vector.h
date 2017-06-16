#ifndef VECTOR_H
#define VECTOR_H

#include <types.h>

#define VEC_INIT(vec, elem_t) (vec_init((vec), sizeof(elem_t)))
#define VEC_ADD(vec, elem) (vec_add((vec), &(elem)))

typedef struct {
  size_t capacity;
  size_t size;
  size_t elem_size;
  char* data;
} vector_t;

int vec_init(vector_t* vector, size_t elem_size);
int vec_resize(vector_t* vector, size_t capacity);
int vec_add(vector_t* vector, void* elem);
void* vec_get(vector_t* vector, int index);
int vec_clear(vector_t* vector);
void vec_free(vector_t* vector);

#endif
