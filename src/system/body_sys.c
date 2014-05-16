#include "system/body_sys.h"

static void update_body(ecs_component *body_comp);
static double elapsed_time = 0;

void body_system_fn(double time) {
  elapsed_time = time;
  list *body_list = ecs_component_store[(int)ECS_COMPONENT_BODY];
  list_each(body_list, (list_lambda)update_body);
}

static void update_body(ecs_component *body_comp) {
  assert(body_comp->type == ECS_COMPONENT_BODY);
  Body *body = &(body_comp->body);              // the component itself
  ecs_entity *entity = body_comp->owner_entity; // the owner entity
  assert(entity != NULL);
  vector displacement = vector_scale(body->velocity, elapsed_time);
  entity->position = vector_add(displacement, entity->position);
}
