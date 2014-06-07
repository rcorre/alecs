#include <string.h>
#include "util/list.h"

list* list_new() {
  return (list*)calloc(1, sizeof(list));
}

void list_free(list *list, list_lambda fn) {
  list_clear(list, fn);
  free(list);
}

void list_clear(list *list, list_lambda fn) {
  while (list->head != NULL) {
    list_remove(list, list->head, fn);
  }
}

list_node* list_push(list *list, void *value) {
  list_node *new = (list_node*)malloc(sizeof(list_node)); 
  new->value = value;        // store the value
  new->next = list->head;   // point new at previous head
  new->prev = NULL;
  if (list->head == NULL) { // was list empty?
    list->tail = new;       // new is also tail
  }
  else {
    list->head->prev = new;
  }
  list->head = new;         // new is new head
  list->length++;
  return new;
}

void* list_popfront(list *list) {
  void *val = NULL;
  if (list->head) {
    val = list->head->value;
    list_remove(list, list->head, NULL);
  }
  return val;
}

list_node* list_remove(list *list, list_node *node, list_lambda fn) {
  list_node *next = node->next;
  list_node *prev = node->prev;
  if (next == NULL) {   // node was list tail
    list->tail = prev; // preceding node is now tail
  }
  else {               // node was not list tail
    next->prev = prev; // point following node at preceding node
  }
  if (prev == NULL) {   // node was list head
    list->head = next; // following node is now head
  }
  else {                // node was not list head
    prev->next = next;  // point preceding node at following node
  }
  if (fn != NULL) {     // was a free function given?
    fn(node->value);    // call free function on value
  }
  free(node);           // destroy node
  list->length--;
  return next;          // return pointer to next node
}

list_node* list_find(list *list, void *value) {
  list_node *node = list->head;
  while (node != NULL && node->value != value) {
    node = node->next;
  }
  return node;
}

int list_count(list *list, void *value) {
  int count = 0;
  for (list_node *node = list->head; node; node = node->next) {
    if (node->value == value) { ++count; }
  }
  return count;
}

void list_each(list *list, list_lambda fn) {
  list_node *node = list->head;
  while (node != NULL) {
    list_node *next = node->next;
    fn(node->value);
    node = next;
  }
}
