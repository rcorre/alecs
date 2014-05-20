#include "system/body_sys.h"

static double elapsed_time = 0; // store update time
static void update_body(ecs_component *body_comp);
// return true if an entity is out of bounds an should be destroyed
static bool out_of_bounds(ecs_entity *e, Body *b);
// keep the linear and angular speed of a Body within its limits
static void limit_speed(Body *b);

void body_system_fn(double time) {
  elapsed_time = time;
  list *body_list = ecs_component_store[(int)ECS_COMPONENT_BODY];
  list_each(body_list, (list_lambda)update_body);
}

void make_constant_vel_body(Body *b, vector vel) {
  b->velocity = vel;
  b->max_linear_velocity = vector_len(vel);
}

static void update_body(ecs_component *body_comp) {
  assert(body_comp->type == ECS_COMPONENT_BODY);
  //assert(body_comp->owner_entity->components[ECS_COMPONENT_BODY] == body_comp);
  Body *body = &(body_comp->body);              // the component itself
  ecs_entity *entity = body_comp->owner_entity; // the owner entity
  assert(entity != NULL);
  limit_speed(body);
  vector displacement = vector_scale(body->velocity, elapsed_time);
  entity->position = vector_add(displacement, entity->position);
  if (out_of_bounds(entity, body)) {
    ecs_entity_free(entity);
  }
}

static bool out_of_bounds(ecs_entity *e, Body *b) {
  int top, bottom, left, right;
  sprite *s = e->sprite;
  if (s != NULL) {  // determine bounds by sprite
    left = e->position.x - sprite_width(s) / 2;
    right = e->position.x + sprite_width(s) / 2;
    top = e->position.y - sprite_height(s) / 2;
    bottom = e->position.y + sprite_height(s) / 2;
  }
  else {  // just use the entity's center point
    top = bottom = e->position.x;
    left = right = e->position.y;
  }

  return
    ((b->destroy_on_exit & NORTH) && bottom < 0) ||
    ((b->destroy_on_exit & WEST ) && right  < 0) ||
    ((b->destroy_on_exit & SOUTH) && top    > SCREEN_H) ||
    ((b->destroy_on_exit & EAST ) && left   > SCREEN_W);
}

static void limit_speed(Body *b) {
  double linear_factor = vector_len(b->velocity) / b->max_linear_velocity;
  if (linear_factor > 1) {
    b->velocity = vector_scale(b->velocity, 1 / linear_factor);
  }
}
