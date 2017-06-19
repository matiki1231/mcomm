#include "vector.h"

#include <stdlib.h>
#include <string.h>

static inline void* _vec_get(vector_t* vector, int index) {
  return vector->data + vector->elem_size * index;
}

bool vec_init(vector_t* vector, size_t elem_size) {
  vector->capacity = VECTOR_INIT_CAPACITY;
  vector->size = 0;
  vector->elem_size = elem_size;
  vector->data = malloc(vector->capacity * elem_size);

  return vector->data != NULL;
}

bool vec_resize(vector_t* vector, size_t capacity) {
  char* data = (char*) realloc(vector->data, vector->elem_size * capacity);
  bool result = data != NULL;

  if (result) {
    vector->capacity = capacity;
    vector->data = data;

    if (vector->capacity < vector->size)
      vector->size = vector->capacity;
  }

  return result;
}

bool vec_add(vector_t* vector, const void* elem) {
  bool result = true;

  if (vector->size == vector->capacity)
    result = vec_resize(vector, vector->size + VECTOR_RESIZE_EXCESS);

  memcpy(_vec_get(vector, vector->size), elem, vector->elem_size);
  ++(vector->size);

  return result;
}

void* vec_get(vector_t* vector, int index) {
  if (index < 0 || index >= vector->size)
    return NULL;

  return _vec_get(vector, index);
}

int vec_find(vector_t* vector, const void* elem) {
  for (int i = 0; i < vector->size; ++i)
    if (memcmp(_vec_get(vector, i), elem, vector->elem_size) == 0)
      return i;

  return -1;
}

bool vec_remove(vector_t* vector, int index) {
  if (index < 0 || index >= vector->size)
    return false;

  --(vector->size);
  memmove(_vec_get(vector, index), _vec_get(vector, index + 1), (vector->size - index) * vector->elem_size);

  if (vector->capacity - vector->size > VECTOR_RESIZE_TRESHOLD)
    vec_resize(vector, vector->size + VECTOR_RESIZE_EXCESS);

  return true;
}

void vec_free(vector_t* vector) {
  if (vector != NULL) {
    free(vector->data);
    vector->elem_size = 0;
    vector->capacity = 0;
    vector->size = 0;
  }
}

