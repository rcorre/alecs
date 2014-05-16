#include "scene/level.h"

static bool run = true;
static ecs_entity *player_ship;

static bool level_update(double time) {
  return run;
}

static void level_draw(void) {
}

static void level_handle_mouse(ALLEGRO_MOUSE_EVENT ev) {
}

static void level_handle_keyboard(ALLEGRO_KEYBOARD_EVENT ev) {
  if (ev.keycode == ALLEGRO_KEY_ESCAPE) {
    run = false;
  }
}

static void level_shutdown() {
}

scene level_new(void) {
  player_ship = ecs_entity_new((vector){.x = 50, .y = 50});
  ecs_attach_sprite(player_ship, "hawk", 0);
  return (scene){
    .update = level_update,
    .draw = level_draw,
    .mouse = level_handle_mouse,
    .keyboard = level_handle_keyboard,
    .shutdown = level_shutdown,
    .bg_color = al_map_rgb(200,200,200)
  };
}
