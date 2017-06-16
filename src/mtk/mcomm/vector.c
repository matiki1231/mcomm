#include "vector.h"

#include <stdlib.h>

int vec_init(vector_t* vector, size_t elem_size) {
  vector->capacity = 0;
  vector->size = 0;
  vector->elem_size = elem_size;
  vector->data = malloc(0);

  return vector->data
}

int vec_resize(vector_t* vector, size_t capacity) {
  char* data = (char*) realloc(vector->data, vector->elem_size * capacity);

  if (data) {
    
  }
}

int vec_add(vector_t* vector, void* elem);
void* vec_get(vector_t* vector, int index);
int vec_clear(vector_t* vector);
void vec_free(vector_t* vector);

