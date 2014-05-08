#include "util/pair.h"
/** create a new pair. key is copied. */
pair *pair_new(const char *key, void *value) {
  pair *p = malloc(sizeof(pair));
  p->key = strdup(key);
  p->value = value;
  return p;
}

/** free a pair. key is freed. value is freed by calling free_fn if provided */
void pair_free(pair *p, void (*free_fn)(void*)) {
  free((void*)p->key);
  if (free_fn != NULL) {
    free_fn(p->value);
  }
  free(p);
}
