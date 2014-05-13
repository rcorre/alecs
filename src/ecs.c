#include "ecs.h"

// externally declared in ecs.h - will be populated and used here
list *ecs_component_store[NUM_COMPONENT_TYPES];
list *ecs_system_update_handlers;
list *ecs_system_draw_handlers;

void ecs_init() {
  for (int i = 0; i < NUM_COMPONENT_TYPES; i++) {
    ecs_component_store[i] = list_new();
  }
  ecs_system_update_handlers = list_new();
  ecs_system_draw_handlers = list_new();
  list_push(ecs_system_update_handlers, sprite_system_draw);
}

ecs_entity *ecs_entity_new(vector position) {
  ecs_entity *entity = calloc(1, sizeof(entity));
  entity->position = position;
  return entity;
}

void ecs_entity_free(ecs_entity *entity) {
  for (ecs_component_type i = 0; i < NUM_COMPONENT_TYPES; i++) {
    ecs_remove_component(entity, i);
  }
  free(entity);
}
struct ecs_component* ecs_add_component(ecs_entity *entity, 
    ecs_component_type type) 
{
  struct ecs_component *comp = calloc(1, sizeof(struct ecs_component));
  comp->type = type;           // tag entity type
  comp->owner_entity = entity; // point component back to owner
  // place component in global store and give it a pointer to its holder
  comp->node = list_push(ecs_component_store[type], comp);
  return comp;
}

void ecs_remove_component(ecs_entity *entity, ecs_component_type type) {
  // get component of given type from entity
  struct ecs_component *comp = entity->components[(int)type];
  // if entity did not have a component of this type, do nothing
  if (comp != NULL) {
    // remove component from global component store and free its memory
    list_remove(ecs_component_store[type], comp->node, free);
    // make sure entity no longer references a component for that type
    entity->components[(int)type] = NULL;
  }
}

void ecs_update_systems(double time) {
  list_node *sys_node = ecs_system_update_handlers->head;
  while (sys_node != NULL) {
    ecs_update_handler handler = (ecs_update_handler)sys_node->value;
    handler(time);
    sys_node = sys_node->next;
  }
}

void ecs_draw_systems(void) {
  list_node *sys_node = ecs_system_draw_handlers->head;
  while (sys_node != NULL) {
    ecs_draw_handler handler = (ecs_draw_handler)sys_node->value;
    handler();
    sys_node = sys_node->next;
  }
}

void ecs_shutdown() {
  list_free(ecs_system_update_handlers, free);
  list_free(ecs_system_draw_handlers, free);
}
