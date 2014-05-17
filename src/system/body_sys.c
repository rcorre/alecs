#include "system/body_sys.h"

static double elapsed_time = 0; // store update time
static void update_body(ecs_component *body_comp);
// return true if an entity is out of bounds an should be destroyed
static bool out_of_bounds(ecs_entity *e, Body *b);

void body_system_fn(double time) {
  elapsed_time = time;
  list *body_list = ecs_component_store[(int)ECS_COMPONENT_BODY];
  list_each(body_list, (list_lambda)update_body);
}

static void update_body(ecs_component *body_comp) {
  assert(body_comp->type == ECS_COMPONENT_BODY);
  //assert(body_comp->owner_entity->components[ECS_COMPONENT_BODY] == body_comp);
  Body *body = &(body_comp->body);              // the component itself
  ecs_entity *entity = body_comp->owner_entity; // the owner entity
  assert(entity != NULL);
  vector displacement = vector_scale(body->velocity, elapsed_time);
  entity->position = vector_add(displacement, entity->position);
  entity->angle += body->angular_velocity * elapsed_time;
  if (out_of_bounds(entity, body)) {
    ecs_entity_free(entity);
  }
}

static bool out_of_bounds(ecs_entity *e, Body *b) {
  rectangle bounds = {.x = e->position.x, .y = e->position.y, .w = 0, .h = 0};
  sprite *s = e->sprite;
  if (s != NULL) {
    bounds.x -= sprite_width(s) / 2;
    bounds.y -= sprite_height(s) / 2;
    bounds.w = sprite_width(s);
    bounds.w = sprite_height(s);
  }

  return
    ((b->destroy_on_exit & NORTH) && rect_bottom(bounds) < 0) ||
    ((b->destroy_on_exit & WEST ) && rect_right(bounds)  < 0) ||
    ((b->destroy_on_exit & SOUTH) && rect_top(bounds)  > SCREEN_H) ||
    ((b->destroy_on_exit & EAST ) && rect_left(bounds) > SCREEN_W);
}
