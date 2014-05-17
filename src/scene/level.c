#include "scene/level.h"

static bool run = true;
static ecs_entity *player_ship;
static ALLEGRO_FONT *main_font;

static bool level_update(double time) {
  return run;
}

static void level_draw(void) {
  al_draw_textf(main_font, al_map_rgb(255,0,0), 0, 0, 0,
      "#entities: %d", ecs_entities->length);
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
  main_font = al_game_get_font("LiberationMono-Regular");
  player_ship = make_player_ship();
  return (scene){
    .update = level_update,
    .draw = level_draw,
    .mouse = level_handle_mouse,
    .keyboard = level_handle_keyboard,
    .shutdown = level_shutdown,
    .bg_color = al_map_rgb(200,200,200)
  };
}
