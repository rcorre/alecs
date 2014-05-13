#ifndef STRINGMAP_H
#define STRINGMAP_H

#include <string.h>
#include <stdlib.h>
#include "util/list.h"

/** \file stringmap.h
  * \brief allows lookup of stored values by a string key
**/

/** key-value pair with a string key */
typedef struct stringmap {
  list *_key_value_pairs;
  list_lambda _free_fn;
} stringmap;

/** create a new \ref stringmap
  * \param free_fn function that will be called to free removed values. Can be
  * NULL to indicate that values do not need to be freed.
  * \return a \ref stringmap with no entries
**/
stringmap *stringmap_new(list_lambda free_fn);
/** release all memory held by a stringmap.
  * \param map \ref stringmap to free
**/
void stringmap_free(stringmap *map);
/** add a new entry to a \ref stringmap
  * \param key string that identifies entry. will be copied. undefined behavior
  * for adding a key that already exists
  * \param val value to associate with the given key. not copied.
**/
void stringmap_add(stringmap *map, const char *key, void *val);
/** remove an entry from a \ref stringmap
  * \param map \ref stringmap from which to remove the entry
  * \param key key of entry to be removed
**/
void stringmap_remove(stringmap *map, const char *key);
/** find a value in a \ref stringmap
  * \param map \ref stringmap in which to find entry
  * \param key key of entry to be locate
  * \return value associated with the key, or NULL if the key is not found
**/
void* stringmap_find(stringmap *map, const char *key);

#endif /* end of include guard: STRINGMAP_H */
