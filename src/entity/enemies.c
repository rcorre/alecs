#include "entity/enemies.h"

ecs_entity* spawn_enemy(vector pos) {
  ecs_entity *enemy = ecs_entity_new(pos);
  ecs_attach_sprite(enemy, "hawk", -1);
  ecs_add_component(enemy, ECS_COMPONENT_BODY);
  Collider *col = &ecs_add_component(enemy, ECS_COMPONENT_COLLIDER)->collider;
  col->rect = hitrect_from_sprite(enemy->sprite);
  col->keep_inside_level = true;
  MouseListener *listener = 
    &ecs_add_component(enemy, ECS_COMPONENT_MOUSE_LISTENER)->mouse_listener;
  listener->click_rect = col->rect;
  listener->on_enter = weapon_set_target;
  listener->on_leave = weapon_clear_target;
  return enemy;
}
