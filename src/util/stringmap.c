#include "util/stringmap.h"
struct stringmap_entry {
  char *key;
  void *value;
};

// internal function to free a key value pair
// map is the map from which the pair should be removed
// node is the node corresponding to that entry in the kvp list
static void sm_free_pair(stringmap *map, list_node *node) {
  struct stringmap_entry *e = (struct stringmap_entry*)node->value;
  free(e->key);                                   // free the entry's key
  map->_free_fn(e->value);                        // free the entry's value
  list_remove(map->_key_value_pairs, node, free); // remove entry from list
}

// internal function to find an entry for a given key in a stringmap
static list_node* sm_find_entry_node(stringmap *map, const char *key) {
  list_node *node = map->_key_value_pairs->head;
  for (; node != NULL; node = node->next) {
    struct stringmap_entry *e = (struct stringmap_entry*)node->value;
    if (strcmp(e->key, key) == 0) {
      return node;
    }
  }
  return NULL;
}

stringmap *stringmap_new(list_lambda free_fn) {
  stringmap *map = malloc(sizeof(stringmap));
  map->_key_value_pairs = list_new();
  map->_free_fn = free_fn;
  return map;
}

void stringmap_free(stringmap *map) {
  list_node *node = map->_key_value_pairs->head;
  while (node != NULL) {
    list_node *temp = node;
    node = node->next;
    sm_free_pair(map, temp);
  }
  free(map);
}

void stringmap_add(stringmap *map, const char *key, void *val) {
  struct stringmap_entry *e = malloc(sizeof(struct stringmap_entry));
  e->key = strdup(key);
  e->value = val;
  list_push(map->_key_value_pairs, e);
}

void stringmap_remove(stringmap *map, const char *key) {
  list_node *node = sm_find_entry_node(map, key);
  if (node != NULL) {  // only try to remove entry if it existed
    sm_free_pair(map, node);
  }
}

void* stringmap_find(stringmap *map, const char *key) {
  list_node *node = sm_find_entry_node(map, key);
  if (node) {  // entry found, return value
    struct stringmap_entry *e = (struct stringmap_entry*)node->value;
    return e->value;
  }
  return NULL;  // entry not found
}
