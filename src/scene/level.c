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
  .lockon_time = 0.1,
  .offset = {0, 10},
  .initial_speed = 100,
  .max_speed = 400,
  .acceleration = 1800,
  .turn_rate = 1.5 * PI,
  .power = 5,
  .radius = 20
};

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
    al_draw_rectangle(r.x, r.y, r.x + r.w, r.y + r.h, al_map_rgb_f(0,1,0), 3);
  }
#endif
}

static void level_handle_mouse(ALLEGRO_MOUSE_EVENT ev) {
  if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
    weapon_fire_player(player_ship); // fire player's current weapon
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
  main_font = al_game_get_font("LiberationMono-Regular");
  player_ship = make_player_ship();
  weapon_system_set_weapons(&current_weapon, &current_weapon);
  scenery_add_background("sunset", -SPRITE_LAYER_LIMIT, 0, 0);
  scenery_add_background("mountains2", -SPRITE_LAYER_LIMIT + 1, 15, 0);
  scenery_add_background("mountains1", -SPRITE_LAYER_LIMIT + 2, 30, 0);
  //TODO: automate
  spawn_enemy((vector){300,100}, WEST, player_ship);
  spawn_enemy((vector){600,100}, NORTH, player_ship);
  spawn_enemy((vector){900,100}, EAST, player_ship);
  spawn_enemy((vector){600,600}, SOUTH, player_ship);
  return (scene){
    .update = level_update,
    .draw = level_draw,
    .mouse = level_handle_mouse,
    .keyboard = level_handle_keyboard,
    .shutdown = level_shutdown,
    .bg_color = al_map_rgb(20,20,20)
  };
}
