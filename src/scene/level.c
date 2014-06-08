#include "scene/level.h"

static const double enemy_time = 3.0;

static bool run = true;
static ecs_entity *player_ship;
static double next_enemy_time;

static bool level_update(double time) {
  next_enemy_time -= time;
  if (next_enemy_time <= 0) {
    //spawn_enemy((vector){300,100}, EAST, player_ship);
    next_enemy_time = enemy_time;
  }
  return run;
}

static void level_draw(void) {
#ifndef NDEBUG
  static char* comp_names[] = {
    "Body",
    "Collider",
    "Propulsion",
    "Health",
    "Timer",
    "Behavior",
    "Keyboard_listener",
    "Mouse_listener"
  };
  al_draw_textf(main_font, al_map_rgb(255,0,0), 0, 0, 0,
      "#entities: %d", ecs_entities->length);
  // component counts
  for (int i = 0; i < NUM_COMPONENT_TYPES; i++) {
    list *comp_list = ecs_component_store[i];
    al_draw_textf(main_font, al_map_rgb(200,0,200), 0, 300 + 40 * i, 0,
        "#%s: %d", comp_names[i], comp_list->length);
  }
  // draw hitrects
  list_node *node = ecs_component_store[ECS_COMPONENT_COLLIDER]->head;
  for (; node; node = node->next) {
    rectangle r = ((ecs_component*)node->value)->collider.rect;
    al_draw_rectangle(r.x, r.y, r.x + r.w, r.y + r.h, al_map_rgba_f(0,1,0,0.5), 2);
  }
#endif
}

static void level_handle_mouse(ALLEGRO_MOUSE_EVENT ev) {
  if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
    if (ev.button == 1) {
      weapon_fire_player(); // fire player's current weapon
    }
    else if (ev.button == 2) {
      weapon_swap(); // swap player's current weapon
    }
  }
}

static void level_handle_keyboard(ALLEGRO_KEYBOARD_EVENT ev) {
  if (ev.keycode == ALLEGRO_KEY_ESCAPE) {
    run = false;
  }
  if (ev.keycode == ALLEGRO_KEY_SPACE) {
    launch_flare(player_ship->position);
  }
}

static void level_shutdown() {
}

scene level_new(void) {
  player_ship = make_player_ship();
  weapon_system_set_weapons(player_ship, &seeker_launcher, &swarmer_launcher);
  scenery_add_background("sunset", -SPRITE_LAYER_LIMIT, 0, 0);
  scenery_pre_populate();
  //TODO: automate
  spawn_enemy((vector){300,100}, WEST, player_ship);
  spawn_enemy((vector){600,100}, NORTH, player_ship);
  spawn_enemy((vector){900,100}, EAST, player_ship);
  spawn_enemy((vector){600,600}, SOUTH, player_ship);
  spawn_mine(EAST, player_ship);
  return (scene){
    .update = level_update,
    .draw = level_draw,
    .mouse = level_handle_mouse,
    .keyboard = level_handle_keyboard,
    .shutdown = level_shutdown,
    .bg_color = al_map_rgb(20,20,20)
  };
}
