#include "entity/player.h"

static void kbd_handler(ecs_entity *e, int keycode, bool down);

/* Functions ******************************************************************/
ecs_entity* make_player_ship() {
  ecs_entity *player = ecs_entity_new((vector){.x = 50, .y = 400}, ENTITY_SHIP);
  ecs_attach_animation(player, "viper", 2, 64, 64, 8, ANIMATE_LOOP);
  // keyboard input listener component
  ecs_component *k = ecs_add_component(player, ECS_COMPONENT_KEYBOARD_LISTENER);
  k->keyboard_listener.handler = kbd_handler;
  // propulsion component
  ecs_component *prop = ecs_add_component(player, ECS_COMPONENT_PROPULSION);
  prop->propulsion.linear_accel = 1500;
  prop->propulsion.turn_rate = PI;
  prop->propulsion.particle_effect = get_particle_generator("player-engine");
  propulsion_assign_sound(player, "jet_engine");
  // body component
  ecs_component *bod = ecs_add_component(player, ECS_COMPONENT_BODY);
  bod->body.max_linear_velocity = 500;
  bod->body.mass = 10;
  bod->body.deceleration_factor = 0.5;
  // collider component
  Collider *col = &ecs_add_component(player, ECS_COMPONENT_COLLIDER)->collider;
  col->rect = hitrect_from_sprite(player->sprite);
  col->keep_inside_level = true;
  col->elastic_collision = true;
  col->collide_particle_effect = get_particle_generator("sparks");
  player->team = TEAM_FRIENDLY;
  return player;
}

static void kbd_handler(ecs_entity *e, int keycode, bool down) {
  Propulsion *p = &e->components[ECS_COMPONENT_PROPULSION]->propulsion;
  vector *throttle = &p->linear_throttle;
  double factor = down ? 1 : -1;
  assert(p != NULL);
  switch(keycode) {
    case ALLEGRO_KEY_W:
      throttle->y += -factor; // move up
      break;
    case ALLEGRO_KEY_S:
      throttle->y += factor;  // move down
      break;
    case ALLEGRO_KEY_A:
      throttle->x += -factor; // move left
      break;
    case ALLEGRO_KEY_D:
      throttle->x += factor;  // move right
      break;
  }
}

/* Weapons ********************************************************************/
Weapon seeker_launcher = {
  .name = "seeker",
  .particle_effect = "seeker-rocket",
  .max_lockons = 5,
  .lockon_time = 0.1,
  .offset = {0, -10},
  .initial_velocity = {0, 100},
  .max_speed = 500,
  .acceleration = 2500,
  .deceleration_factor = 0.3,
  .turn_rate = 1.5 * PI,
  .power = 5,
  .fire_delay = 0.33,
  .radius = 20,
};

Weapon destroyer_launcher = {
  .name = "destroyer",
  .particle_effect = "destroyer-rocket",
  .max_lockons = 5,
  .lockon_time = 0.4,
  .offset = {0, 10},
  .initial_velocity = {-300, 400},
  .max_speed = 1000,
  .acceleration = 800,
  .deceleration_factor = 1.3,
  .turn_rate = 6.0 * PI,
  .power = 10,
  .fire_delay = 0.33,
  .radius = 40
};

Weapon swarmer_launcher = {
  .name = "swarmer",
  .particle_effect = "swarmer-rocket",
  .max_lockons = 15,
  .lockon_time = 0.05,
  .offset = {0, -10},
  .initial_velocity = {300, 400},
  .max_speed = 500,
  .acceleration = 2000,
  .deceleration_factor = 0.2,
  .turn_rate = 1.2 * PI,
  .power = 5,
  .fire_delay = 0.03,
  .radius = 15,
  .fire_fn = fire_swarmer_pod
};
