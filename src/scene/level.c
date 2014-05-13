#include "scene/level.h"

static bool run = true;
static ALLEGRO_BITMAP *bmp;
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
  al_destroy_bitmap(bmp);
}

scene level_new(void) {
  bmp = al_game_get_bitmap("hawk");
  player_ship = ecs_entity_new((vector){.x = 50, .y = 50});
  ecs_component *sprite_comp = ecs_add_component(player_ship, COMP_SPRITE);
  sprite *s = &sprite_comp->sprite;
  s->bitmap = bmp;
  s->center = (vector){.x = 20, .y = 20};
  s->tint = al_map_rgb(255, 255, 255);
  s->scale = 1.0;
  return (scene){
    .update = level_update,
    .draw = level_draw,
    .mouse = level_handle_mouse,
    .keyboard = level_handle_keyboard,
    .shutdown = level_shutdown
  };
}
