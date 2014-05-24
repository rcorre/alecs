#include "entity/enemies.h"

static const double min_fire_time = 2;
static const double max_fire_time = 5;

static void fire_at_player(ecs_entity *enemy, void *player) {
  weapon_fire_enemy(enemy, player);
  // reset fire timer
  Timer *timer = &enemy->components[ECS_COMPONENT_TIMER]->timer;
  timer->time_left = randd(min_fire_time, max_fire_time);
}

ecs_entity* spawn_enemy(vector pos, Direction enter_from, ecs_entity *player) {
  vector start;
  switch (enter_from) {
    case EAST:
      start = (vector){SCREEN_W + 100, pos.y};
      break;
    case WEST:
      start = (vector){-100, pos.y};
      break;
    case NORTH:
      start = (vector){pos.x, -100};
      break;
    case SOUTH:
      start = (vector){pos.x, SCREEN_H + 100};
      break;
    default:
      start = pos;
  }
  ecs_entity *enemy = ecs_entity_new((vector)start);
  // sprite
  ecs_attach_animation(enemy, "enemy1", 1, 64, 24, 6, ANIMATE_LOOP);
  // body
  Body *bod = &ecs_add_component(enemy, ECS_COMPONENT_BODY)->body;
  bod->max_linear_velocity = 200;
  // collider
  Collider *col = &ecs_add_component(enemy, ECS_COMPONENT_COLLIDER)->collider;
  col->rect = hitrect_from_sprite(enemy->sprite);
  col->keep_inside_level = true;
  // mouse listener
  MouseListener *listener =
    &ecs_add_component(enemy, ECS_COMPONENT_MOUSE_LISTENER)->mouse_listener;
  listener->click_rect = col->rect;
  listener->on_enter = weapon_set_target;
  listener->on_leave = weapon_clear_target;
  // propulsion
  Propulsion *pro = &ecs_add_component(enemy, ECS_COMPONENT_PROPULSION)->propulsion;
  pro->linear_accel = 100;
  pro->turn_rate = 2*PI;
  // behavior
  Behavior *beh = &ecs_add_component(enemy, ECS_COMPONENT_BEHAVIOR)->behavior;
  beh->target = player;
  beh->type = BEHAVIOR_MOVE;
  beh->location = pos;
  enemy->team = TEAM_ENEMY;
  Timer *timer = &ecs_add_component(enemy, ECS_COMPONENT_TIMER)->timer;
  timer->time_left = randd(min_fire_time, max_fire_time);
  timer->timer_delegate = (ecs_entity_delegate) {
    .delegate = fire_at_player,
    .data = player
  };
  return enemy;
}
