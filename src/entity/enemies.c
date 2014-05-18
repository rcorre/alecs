#include "entity/enemies.h"

ecs_entity* spawn_enemy(vector pos) {
  ecs_entity *enemy = ecs_entity_new(pos);
  ecs_attach_sprite(enemy, "hawk", -1);
  ecs_add_component(enemy, ECS_COMPONENT_BODY);
  Collider *col = &ecs_add_component(enemy, ECS_COMPONENT_COLLIDER)->collider;
  col->rect = hitrect_from_sprite(enemy->sprite);
  col->keep_inside_level = true;
  Health *health = &ecs_add_component(enemy, ECS_COMPONENT_HEALTH)->health;
  return enemy;
}
