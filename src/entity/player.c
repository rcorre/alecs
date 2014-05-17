#include "entity/player.h"

static void kbd_handler(ecs_entity *e, int keycode, bool down);

ecs_entity* make_player_ship() {
  ecs_entity *player = ecs_entity_new((vector){.x = 50, .y = 50});
  ecs_attach_sprite(player, "hawk", 0);
  ecs_component *k = ecs_add_component(player, ECS_COMPONENT_KEYBOARD_LISTENER);
  k->keyboard_listener.handler = kbd_handler;
  ecs_component *prop = ecs_add_component(player, ECS_COMPONENT_PROPULSION);
  prop->propulsion.linear_accel = 400;
  prop->propulsion.angular_accel = PI;
  ecs_component *bod = ecs_add_component(player, ECS_COMPONENT_BODY);
  bod->body.max_linear_velocity = 800;
  bod->body.max_angular_velocity = PI;
  bod->body.mass = 10;
  return player;
}

static void kbd_handler(ecs_entity *e, int keycode, bool down) {
  int factor = down ? 1 : 0; // down means throttle on, up means off
  Propulsion *p = &e->components[ECS_COMPONENT_PROPULSION]->propulsion;
  assert(p != NULL);
  switch(keycode) {
    case ALLEGRO_KEY_W:
      p->linear_throttle.y = -factor; // move up
      break;
    case ALLEGRO_KEY_S:
      p->linear_throttle.y = factor;  // move down
      break;
    case ALLEGRO_KEY_A:
      p->linear_throttle.x = -factor; // move left
      break;
    case ALLEGRO_KEY_D:
      p->linear_throttle.x = factor;  // move right
      break;
  }
}
