#ifndef CONCURR_VECTOR_H
#define CONCURR_VECTOR_H

#include <pthread.h>

#include "vector.h"

typedef struct {
  vector_t underlying;
  pthread_mutex_t modify;
  pthread_mutex_t empty;
} cvector_t;

bool cvec_init(cvector_t* vector, size_t elem_size);
bool cvec_resize(cvector_t* vector, size_t capacity);
bool cvec_add(cvector_t* vector, const void* elem);
int cvec_find(cvector_t* vector, const void* elem);
bool cvec_remove(cvector_t* vector, int index);
bool cvec_fremove(cvector_t* vector, const void* elem);
void cvec_free(cvector_t* vector);
void cvec_foreach(cvector_t* vector, void (*action)(void*, void*), void* capture);
void cvec_flush(cvector_t* vector, void (*action)(void*, void*), void* capture);
bool cvec_clear(cvector_t* vector);

#endif
