#ifndef PAIR_H
#define PAIR_H

#include <string.h>
#include <stdlib.h>

/** \file pair.h
  * \brief a simple key-value pair. Used for storing component templates.
**/

/** key-value pair with a string key */
typedef struct pair {
  const char *key;
  void *value;
} pair;

/** create a new pair.
 *  \param key string key of pair - copied into constructed pair.
 *  \param value value of pair - not copied.*/
pair *pair_new(const char *key, void *value);
/** free a pair.
 *  \param p pair to free
 *  \param free_fn if free_fn != NULL, it is called to free p->value
 **/
void pair_free(pair *p, void (*free_fn)(void*));

#endif /* end of include guard: PAIR_H */
