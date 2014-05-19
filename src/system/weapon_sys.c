#include "system/weapon_sys.h"

static const float indicator_radius = 12;
static const float indicator_thickness = 3;

static ecs_entity *current_target;
static double current_lockon_time;
static list *lockon_list;

void weapon_system_fn(double time) {
  if (current_target != NULL) {
    current_lockon_time += time;
  }
}

void weapon_system_draw() {
  if (current_target != NULL) {
    al_draw_arc(current_target->position.x, current_target->position.y,
        indicator_radius, 0, current_lockon_time * 25, al_map_rgb(255,0,0),
        indicator_thickness);
  }
}

void weapon_set_target(ecs_entity *target) {
  if (current_target != target) {
    current_lockon_time = 0;  // new target
    current_target = target;
  }
}

void weapon_clear_target(ecs_entity *target) {
  if (current_target == target) {
    current_lockon_time = 0;  // new target
    current_target = NULL;
  }
}
