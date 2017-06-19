#include "concurr_vector.h"

#include "vector.h"

bool cvec_init(cvector_t* vector, size_t elem_size) {
  pthread_mutex_init(&vector->modify, NULL);
  pthread_mutex_init(&vector->empty, NULL);
  pthread_mutex_lock(&vector->empty);

  return vec_init(&vector->underlying, elem_size);
}

bool cvec_resize(cvector_t* vector, size_t capacity) {
  bool result;
  
  pthread_mutex_lock(&vector->modify);
  
  result = vec_resize(&vector->underlying, capacity);

  if (vector->underlying.size == 0)
    pthread_mutex_trylock(&vector->empty);

  pthread_mutex_unlock(&vector->modify);

  return result;
}

bool cvec_add(cvector_t* vector, const void* elem) {
  bool result;

  pthread_mutex_lock(&vector->modify);

  result = vec_add(&vector->underlying, elem);
  pthread_mutex_trylock(&vector->empty);
  pthread_mutex_unlock(&vector->empty);

  pthread_mutex_unlock(&vector->modify);

  return result;
}

int cvec_find(cvector_t* vector, const void* elem) {
  int result;

  pthread_mutex_lock(&vector->modify);
  result = vec_find(&vector->underlying, elem);
  pthread_mutex_unlock(&vector->modify);

  return result;
}

bool cvec_remove(cvector_t* vector, int index) {
  bool result;

  pthread_mutex_lock(&vector->modify);
  result = vec_remove(&vector->underlying, index);
  pthread_mutex_unlock(&vector->modify);

  return result;
}

bool cvec_fremove(cvector_t* vector, const void* elem) {
  bool result;

  pthread_mutex_lock(&vector->modify);
  result = vec_remove(&vector->underlying, vec_find(&vector->underlying, elem));
  pthread_mutex_unlock(&vector->modify);

  return result;
}

void cvec_free(cvector_t* vector) {
  vec_free(&vector->underlying);
  pthread_mutex_destroy(&vector->modify);
  pthread_mutex_destroy(&vector->empty);
}

void cvec_foreach(cvector_t* vector, void (*action)(void*, void*), void* capture) {
  pthread_mutex_lock(&vector->modify);

  for (int i = 0; i < vector->underlying.size; ++i)
    action(vec_get(&vector->underlying, i), capture);

  pthread_mutex_unlock(&vector->modify);
}

void cvec_flush(cvector_t* vector, void (*action)(void*, void*), void* capture) {
  pthread_mutex_lock(&vector->empty);
  pthread_mutex_lock(&vector->modify);

  for (int i = 0; i < vector->underlying.size; ++i)
    action(vec_get(&vector->underlying, i), capture);

  vec_clear(&vector->underlying);

  pthread_mutex_unlock(&vector->modify);
}

bool cvec_clear(cvector_t* vector) {
  return cvec_resize(vector, 0);
}

