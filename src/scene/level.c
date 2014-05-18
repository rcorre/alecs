#include "scene/level.h"

static const double enemy_time = 1.0;

static bool run = true;
static ecs_entity *player_ship;
static ALLEGRO_FONT *main_font;
static double next_enemy_time;

static bool level_update(double time) {
  next_enemy_time -= time;
  if (next_enemy_time <= 0) {
    spawn_enemy(rand_point((rectangle){0,0,SCREEN_W, SCREEN_H}));
    next_enemy_time = enemy_time;
  }
  return run;
}

static void level_draw(void) {
#ifndef NDEBUG
  al_draw_textf(main_font, al_map_rgb(255,0,0), 0, 0, 0,
      "#entities: %d", ecs_entities->length);
#endif
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
