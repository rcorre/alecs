#ifndef LIST_H
#define LIST_H
#include <stdlib.h>

/** \file list.h
  * \brief A basic doubly-linked list
**/

/* Types -------------------------------------------------------------------- */
/** \brief data container for a list */
typedef struct list_node {
  struct list_node *next, *prev;
  void *value;
} list_node;
/** \brief doubly-linked list */
typedef struct list {
  list_node *head, *tail;
  int length;
} list;

/** \brief function that can be applied to each node of a list */
typedef void (*list_lambda)(void*);
/* -------------------------------------------------------------------------- */

/* Methods------------------------------------------------------------------- */
/** \brief create a new list */
list* list_new(void);
/** destroy a list and all of its nodes.
  * if fn is not NULL, call it on the value of every node */
void list_free(list *list, list_lambda fn);
/** remove all of a list's elements by calling \ref fn (if fn is not NULL) */
void list_clear(list *list, list_lambda fn);
/** \brief destroy a list and all of its nodes */
/** \brief if fn is not NULL, call it on the value of every node */
void list_free(list *list, list_lambda fn);
/** \brief prepend node containing value to list */
list_node* list_push(list *list, void *value);
/** \brief remove and retrieve head of list */
void* list_popfront(list *list);
/** \brief remove node from list.
    if fn is not NULL, call fn on every value. Else call free on every value.
    returns pointer to next node
 */
list_node* list_remove(list *list, list_node *node, list_lambda fn);
/** \brief find node in list whose value matches value (pointer comparison) */
list_node* list_find(list *list, void *value);
/** \brief apply fn to each value in the list */
void list_each(list *list, list_lambda fn);
/* -------------------------------------------------------------------------- */

#endif /* end of include guard: LIST_H */
