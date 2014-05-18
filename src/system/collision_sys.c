#include "system/collision_sys.h"

static void try_boundary_collision(ecs_entity *entity, Collider *collider);

void collision_system_fn(double time) {
  list *collider_list = ecs_component_store[ECS_COMPONENT_COLLIDER];
  list_node *node = collider_list->head;
  for(; node != NULL ; node = node->next) {
    ecs_component *comp = node->value;
    assert(comp->type == ECS_COMPONENT_COLLIDER);
    ecs_entity *entity = comp->owner_entity;    // entity owning collider
    Collider *collider = &comp->collider; // collider component
    if (collider->keep_inside_level) {
      try_boundary_collision(entity, collider);
    }
  }
}

rectangle hitrect_from_sprite(sprite *sprite) {
  // return rect the size of the scaled sprite
  return (rectangle) { .w = sprite_width(sprite), .h = sprite_height(sprite) };
}

static void try_boundary_collision(ecs_entity *entity, Collider *collider) {
  ecs_component *body_comp = entity->components[ECS_COMPONENT_BODY];
  assert(body_comp != NULL && body_comp->type == ECS_COMPONENT_BODY);
  Body *body = &body_comp->body;
  vector* center = &entity->position;
  rectangle hitrect = collider->rect;
  int left   = center->x - hitrect.w / 2;
  int right  = center->x + hitrect.w / 2;
  int top    = center->y - hitrect.h / 2;
  int bottom = center->y + hitrect.h / 2;
  if (left <= 0) {
    center->x -= left;
    body->velocity.x = 0;
  }
  if (right >= SCREEN_W) {
    center->x -= right - SCREEN_W;
    body->velocity.x = 0;
  }
  if (top <= 0) {
    center->y -= top;
    body->velocity.y = 0;
  }
  if (bottom >= SCREEN_H) {
    center->y -= bottom - SCREEN_H;
    body->velocity.y = 0;
  }
}
