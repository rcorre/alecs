#include "ecs.h"

// externally declared in ecs.h - will be populated and used here
list *ecs_systems;
list *ecs_entities;
list *ecs_component_store[NUM_COMPONENT_TYPES];

void ecs_init() {
  sprite_init();
  ecs_systems = list_new();
  ecs_entities = list_new();
  for (int i = 0; i < NUM_COMPONENT_TYPES; i++) {
    ecs_component_store[i] = list_new();
  }
}

ecs_entity* ecs_entity_new(vector position) {
  ecs_entity *entity = calloc(1, sizeof(ecs_entity));
  entity->position = position;
  list_push(ecs_entities, entity); // push onto entity list
  return entity;
}

void ecs_entity_free(ecs_entity *entity) {
  ecs_remove_sprite(entity);
  for (ecs_component_type i = 0; i < NUM_COMPONENT_TYPES; i++) {
    ecs_remove_component(entity, i);
  }
  free(entity);
}

ecs_component* ecs_add_component(ecs_entity *entity, ecs_component_type type) {
  struct ecs_component *comp = calloc(1, sizeof(struct ecs_component));
  comp->type = type;           // tag entity type
  comp->owner_entity = entity; // point component back to owner
  // place component in global store and give it a pointer to its holder
  return comp;
}

void ecs_remove_component(ecs_entity *entity, ecs_component_type type) {
  // get component of given type from entity
  ecs_component *comp = entity->components[(int)type];
  // if entity did not have a component of this type, do nothing
  if (comp != NULL) {
    // remove component from global component store and free its memory
    list_remove(ecs_component_store[type], comp->node, free);
    // make sure entity no longer references a component for that type
    entity->components[(int)type] = NULL;
  }
}

void ecs_attach_sprite(ecs_entity *entity, const char *name, int depth) {
  assert(entity->sprite == NULL); // shouldn't have sprite already
  entity->sprite = sprite_new(name, &(entity->position), &(entity->angle), depth);
}

void ecs_remove_sprite(ecs_entity *entity) {
  if (entity->sprite != NULL) {
    sprite_free(entity->sprite);
    entity->sprite = NULL;
  }
}

void ecs_update_systems(double time) {
  list_node *sys_node = ecs_systems->head;
  while (sys_node != NULL) {  // iterate through every update function
    ecs_system sys = (ecs_system)sys_node->value;
    sys(time);  // run the system update function, passing the elapsed time
    sys_node = sys_node->next;
  }
}

void ecs_free_all_entities() {
  // free every entity without destroying the list
  list_each(ecs_entities, (list_lambda)ecs_entity_free);
}

void ecs_shutdown() {
  list_free(ecs_systems, free);
  list_free(ecs_entities, (list_lambda)ecs_entity_free);
}
