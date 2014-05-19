#include "entity/player.h"

static void kbd_handler(ecs_entity *e, int keycode, bool down);

ecs_entity* make_player_ship() {
  ecs_entity *player = ecs_entity_new((vector){.x = 50, .y = 50});
  ecs_attach_sprite(player, "hawk", 0);
  // keyboard input listener component
  ecs_component *k = ecs_add_component(player, ECS_COMPONENT_KEYBOARD_LISTENER);
  k->keyboard_listener.handler = kbd_handler;
  // propulsion component
  ecs_component *prop = ecs_add_component(player, ECS_COMPONENT_PROPULSION);
  prop->propulsion.linear_accel = 1500;
  prop->propulsion.angular_accel = PI;
  prop->propulsion.particle_effect = get_particle_generator("particles1");
  // body component
  ecs_component *bod = ecs_add_component(player, ECS_COMPONENT_BODY);
  bod->body.max_linear_velocity = 500;
  bod->body.max_angular_velocity = PI;
  bod->body.mass = 10;
  // collider component
  Collider *col = &ecs_add_component(player, ECS_COMPONENT_COLLIDER)->collider;
  col->rect = hitrect_from_sprite(player->sprite);
  col->keep_inside_level = true;
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
