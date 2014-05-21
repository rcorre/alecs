#include "system/collision_sys.h"

static void try_boundary_collision(ecs_entity *entity, Collider *collider);
static void try_entity_collision(ecs_entity *e1, Collider *c1, ecs_entity *e2,
    Collider *c2);

void collision_system_fn(double time) {
  list_node *node = ecs_component_store[ECS_COMPONENT_COLLIDER]->head;
  while(node) {
    list_node *temp = node->next;
    ecs_component *comp = node->value;
    assert(comp->type == ECS_COMPONENT_COLLIDER);
    ecs_entity *entity = comp->owner_entity;    // entity owning collider
    Collider *collider = &comp->collider; // collider component
    collider->rect.x = entity->position.x - collider->rect.w / 2.0;
    collider->rect.y = entity->position.y - collider->rect.h / 2.0;
    if (collider->keep_inside_level) {
      try_boundary_collision(entity, collider);
    }
    // check collision against other colliders
    list_node *other = node->next;
    while(other) {
      list_node *temp_other = other->next;
      ecs_component *other_comp = other->value;
      assert(comp->type == ECS_COMPONENT_COLLIDER);
      Collider *other_col = &other_comp->collider;
      ecs_entity *other_entity = other_comp->owner_entity;
      if (!(ecs_same_team(entity, other_entity))) {
        try_entity_collision(entity, collider, other_entity, other_col);
      }
      // check if the current collider was removed from the current entity
      if (entity->components[ECS_COMPONENT_COLLIDER] != comp) {break;}
      other = temp_other;
    }
    node = temp;
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

static void try_entity_collision(ecs_entity *e1, Collider *c1, ecs_entity *e2,
    Collider *c2)
{
  if (rect_intersect(c1->rect, c2->rect)) {
    // run collision handlers if they exist
    if (c1->on_collision) {c1->on_collision(e1);}
    if (c2->on_collision) {c2->on_collision(e2);}
  }
}
