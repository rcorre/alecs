#include "scene/level.h"

static const double enemy_time = 3.0;

static bool run = true;
static ecs_entity *player_ship;
static ALLEGRO_FONT *main_font;
static double next_enemy_time;

// TODO: testing only
static Weapon current_weapon = {
  .name = "seeker",
  .particle_effect = "seeker-rocket",
  .max_lockons = 5,
  .lockon_time = 0.5,
  .initial_speed = 100,
  .max_speed = 300,
  .acceleration = 1200,
  .turn_rate = 2 * PI,
  .power = 5,
  .radius = 20
};

static bool level_update(double time) {
  next_enemy_time -= time;
  if (next_enemy_time <= 0) {
    spawn_enemy((vector){300,100}, EAST, player_ship);
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
  if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
    weapon_fire(player_ship);
  }
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
  weapon_system_set_weapons(&current_weapon, &current_weapon);
  scenery_add_background("sunset", -SPRITE_LAYER_LIMIT, 0, 0);
  scenery_add_background("mountains2", -SPRITE_LAYER_LIMIT + 1, 15, 0);
  scenery_add_background("mountains1", -SPRITE_LAYER_LIMIT + 2, 30, 0);
  return (scene){
    .update = level_update,
    .draw = level_draw,
    .mouse = level_handle_mouse,
    .keyboard = level_handle_keyboard,
    .shutdown = level_shutdown,
    .bg_color = al_map_rgb(20,20,20)
  };
}
