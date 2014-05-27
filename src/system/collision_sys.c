#include "system/collision_sys.h"

static const double rollback_granularity = 10;

// handle collision with level boundaries
static void try_boundary_collision(ecs_entity *entity, Collider *collider);
// check for and handle collision between two entities
static void try_entity_collision(ecs_entity *e1, Collider *c1, ecs_entity *e2,
    Collider *c2, double time);
// roll back time and replay more slowly when a collision detected
// return time left after collision
static double roll_back_collision(ecs_entity *e1, Body *b1, Collider *c1,
    ecs_entity *e2, Body *b2, Collider *c2, double elapsed_time);
// effect an elastic collision between bodies. called by try_entity_collision
static void elastic_collision(Body *bod1, Body *bod2);

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
      ecs_entity *other_entity = other_comp->owner_entity;
      Collider *other_col = &other_comp->collider;
      other_col->rect.x = other_entity->position.x - other_col->rect.w / 2.0;
      other_col->rect.y = other_entity->position.y - other_col->rect.h / 2.0;
      if (!(ecs_same_team(entity, other_entity))) {
        try_entity_collision(entity, collider, other_entity, other_col, time);
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
    Collider *c2, double time)
{
  if (rect_intersect(c1->rect, c2->rect)) {
    if (c1->elastic_collision && c2->elastic_collision) {
      ecs_component *body_comp1 = e1->components[ECS_COMPONENT_BODY];
      ecs_component *body_comp2 = e2->components[ECS_COMPONENT_BODY];
      assert(body_comp1 && body_comp2);
      Body *bod1 = &body_comp1->body;
      Body *bod2 = &body_comp2->body;
      double t_left = roll_back_collision(e1, bod1, c1, e2, bod2, c2, time);
      elastic_collision(bod1, bod2);
      e1->position = vector_add(e1->position, vector_scale(bod1->velocity, t_left));
      e2->position = vector_add(e2->position, vector_scale(bod2->velocity, t_left));
      // play particle effects if they exist
      if (c1->collide_particle_effect.data != NULL) {
        c1->collide_particle_effect.position = e1->position;
        spawn_particles(&c1->collide_particle_effect, time, 1, ZEROVEC);
      }
      if (c2->collide_particle_effect.data != NULL) {
        c2->collide_particle_effect.position = e2->position;
        spawn_particles(&c2->collide_particle_effect, time, 1, ZEROVEC);
      }
    }
      // run collision handlers if they exist
    if (c1->on_collision) {
      c1->on_collision(e1, e2);
    }
    if (c2->on_collision) {
      c2->on_collision(e2, e1);
    }
  }
}

static void elastic_collision(Body *bod1, Body *bod2) {
  vector v1 = bod1->velocity;
  vector v2 = bod2->velocity;
  double m1 = bod1->mass;
  double m2 = bod2->mass;
  // v1 = (v1 * (m1 - m2) + 2 * m2 * v2) / (m1 + m2)
  // v2 = (v2 * (m2 - m1) + 2 * m1 * v1) / (m1 + m2)
  bod1->velocity = vector_scale(vector_add(vector_scale(v1, m1 - m2),
        vector_scale(v2, 2 * m2)), 1 / (m1 + m2));
  bod2->velocity = vector_scale(vector_add(vector_scale(v2, m2 - m1),
        vector_scale(v1, 2 * m1)), 1 / (m1 + m2));
}

static double roll_back_collision(ecs_entity *e1, Body *b1, Collider *c1,
    ecs_entity *e2, Body *b2, Collider *c2, double elapsed_time)
{
  vector v1 = b1->velocity;
  vector v2 = b2->velocity;
  double r1x = c1->rect.x - v1.x * elapsed_time;
  double r1y = c1->rect.y - v1.y * elapsed_time;
  double r2x = c2->rect.x - v2.x * elapsed_time;
  double r2y = c2->rect.y - v2.y * elapsed_time;
  double step = elapsed_time / rollback_granularity;
  rectangle rect1 = {.x = r1x, .y = r1y, .w = c1->rect.w, .h = c1->rect.h};
  rectangle rect2 = {.x = r2x, .y = r2y, .w = c2->rect.w, .h = c2->rect.h};
  while (!rect_intersect(rect1, rect2) && elapsed_time > 0) {
    elapsed_time -= step;
    r1x += v1.x * step;
    r1y += v1.y * step;
    r2x += v2.x * step;
    r2y += v2.y * step;
    rect1.x = r1x;
    rect1.y = r1y;
    rect2.x = r1x;
    rect2.y = r1y;
  }
  e1->position = (vector){r1x + c1->rect.w / 2, r1y + c1->rect.h / 2};
  e2->position = (vector){r2x + c2->rect.w / 2, r2y + c2->rect.h / 2};
  return elapsed_time;
}
