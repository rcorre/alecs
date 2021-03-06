#include "entity/enemies.h"

static const double min_fire_time = 5;
static const double max_fire_time = 9;

static void fire_at_player(ecs_entity *enemy) {
  ecs_component *behavior_comp = enemy->components[ECS_COMPONENT_BEHAVIOR];
  assert(behavior_comp);
  ecs_entity *player = behavior_comp->behavior.target;
  weapon_fire_enemy(enemy, player);
  // reset fire timer
  Timer *timer = &enemy->components[ECS_COMPONENT_TIMER]->timer;
  timer->time_left = randd(min_fire_time, max_fire_time);
}

static void asplode_enemy(ecs_entity *enemy) {
  scenery_make_explosion(enemy->position, (vector){6,4}, 40,
      al_map_rgb(255,160,160), "explosion1"); 
  ecs_entity_free(enemy);
}

static void start_crashing(ecs_entity *enemy) {
  ecs_remove_component(enemy, ECS_COMPONENT_BEHAVIOR);
  Propulsion *p = &enemy->components[ECS_COMPONENT_PROPULSION]->propulsion;
  p->linear_throttle = (vector){-0.8, 0.8};
  p->angular_throttle = -1;
  Timer *timer = &ecs_add_component(enemy, ECS_COMPONENT_TIMER)->timer;
  timer->time_left = 2;
  timer->timer_action = asplode_enemy;
}

void spawn_enemy(EnemySpawnData data) {
  ecs_entity *enemy = ecs_entity_new(data.start, ENTITY_SHIP);
  // sprite
  ecs_attach_animation(enemy, "enemy1", 1, 64, 24, 6, ANIMATE_LOOP);
  // body
  Body *bod = &ecs_add_component(enemy, ECS_COMPONENT_BODY)->body;
  bod->max_linear_velocity = 200;
  bod->mass = 10;
  // collider
  Collider *col = &ecs_add_component(enemy, ECS_COMPONENT_COLLIDER)->collider;
  col->rect = hitrect_from_sprite(enemy->sprite);
  col->elastic_collision = true;
  col->collide_particle_effect = get_particle_generator("sparks");
  // mouse listener
  MouseListener *listener =
    &ecs_add_component(enemy, ECS_COMPONENT_MOUSE_LISTENER)->mouse_listener;
  listener->click_rect = col->rect;
  listener->on_enter = weapon_set_target;
  listener->on_leave = weapon_clear_target;
  // propulsion
  Propulsion *pro = &ecs_add_component(enemy, ECS_COMPONENT_PROPULSION)->propulsion;
  pro->linear_accel = 100;
  pro->turn_rate = 1*PI;
  // behavior
  Behavior *beh = &ecs_add_component(enemy, ECS_COMPONENT_BEHAVIOR)->behavior;
  beh->target = data.player;
  beh->type = BEHAVIOR_MOVE;
  beh->location = data.target;
  enemy->team = TEAM_ENEMY;
  Timer *timer = &ecs_add_component(enemy, ECS_COMPONENT_TIMER)->timer;
  timer->time_left = randd(min_fire_time, max_fire_time);
  timer->timer_action = fire_at_player;
  ecs_component *health_comp = ecs_add_component(enemy, ECS_COMPONENT_HEALTH);
  health_comp->health = make_health(10, start_crashing, "smoke");
}

static void mine_collide(ecs_entity *mine, ecs_entity *player) {
  deal_damage(player, 10);
  asplode_enemy(mine);
}

ecs_entity* spawn_mine(Direction enter_from, ecs_entity *player) {
  vector start = {SCREEN_W, 300};
  ecs_entity *mine = ecs_entity_new((vector)start, ENTITY_SHIP);
  // sprite
  ecs_attach_sprite(mine, "mine", 3);
  // body
  Body *bod = &ecs_add_component(mine, ECS_COMPONENT_BODY)->body;
  bod->max_linear_velocity = 200;
  bod->mass = 10;
  // collider
  Collider *col = &ecs_add_component(mine, ECS_COMPONENT_COLLIDER)->collider;
  col->rect = hitrect_from_sprite(mine->sprite);
  col->on_collision = mine_collide;
  // mouse listener
  MouseListener *listener =
    &ecs_add_component(mine, ECS_COMPONENT_MOUSE_LISTENER)->mouse_listener;
  listener->click_rect = col->rect;
  listener->on_enter = weapon_set_target;
  listener->on_leave = weapon_clear_target;
  // propulsion
  Propulsion *pro = &ecs_add_component(mine, ECS_COMPONENT_PROPULSION)->propulsion;
  pro->linear_accel = 300;
  pro->turn_rate = 1*PI;
  // behavior
  Behavior *beh = &ecs_add_component(mine, ECS_COMPONENT_BEHAVIOR)->behavior;
  beh->target = player;
  beh->type = BEHAVIOR_FOLLOW;
  mine->team = TEAM_ENEMY;
  Timer *timer = &ecs_add_component(mine, ECS_COMPONENT_TIMER)->timer;
  timer->time_left = 10;
  timer->timer_action = asplode_enemy;
  ecs_component *health_comp = ecs_add_component(mine, ECS_COMPONENT_HEALTH);
  health_comp->health = make_health(10, start_crashing, "smoke");
  return mine;
}
