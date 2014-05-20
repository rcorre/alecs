#include "system/weapon_sys.h"

static const float indicator_radius = 12;
static const float indicator_thickness = 3;

#define PRIMARY_LOCK_COLOR al_map_rgba(128, 0, 0, 128)
#define SECONDARY_LOCK_COLOR al_map_rgba(0, 0, 128, 128)

static struct ecs_entity *current_target;
static double current_lockon_time;
static list *lockon_list;
static struct ecs_entity *firing_entity;
static Weapon *current_weapon, *alternate_weapon;

static void fire_at_target(struct ecs_entity *ent);

void weapon_system_fn(double time) {
  if (current_target != NULL) {
    current_lockon_time += time;
    if (current_lockon_time > current_weapon->lockon_time) {
      list_push(lockon_list, current_target);
      weapon_clear_target(current_target);
    }
  }
}

void weapon_system_draw() {
  if (current_target != NULL) {
    al_draw_arc(current_target->position.x, current_target->position.y,
        indicator_radius, 0, current_lockon_time * 25, PRIMARY_LOCK_COLOR,
        indicator_thickness);
  }
}

void weapon_set_target(struct ecs_entity *target) {
  if (current_target != target) {
    current_lockon_time = 0;  // new target
    current_target = target;
  }
}

void weapon_clear_target(struct ecs_entity *target) {
  if (current_target == target) {
    current_lockon_time = 0;  // new target
    current_target = NULL;
  }
}

void weapon_system_set_weapons(Weapon *primary, Weapon *secondary) {
  if (!lockon_list) { lockon_list = list_new(); }
  current_weapon = primary;
  alternate_weapon = secondary;
}

void weapon_fire(struct ecs_entity *fired_by) {
  firing_entity = fired_by;
  // fire at each target, then remove it from the list
  list_clear(lockon_list, (list_lambda)fire_at_target);
}

void weapon_swap() {
  Weapon *temp = current_weapon;
  current_weapon = alternate_weapon;
  alternate_weapon = temp;
  list_clear(lockon_list, NULL);
  weapon_clear_target(current_target);
}

static void fire_at_target(struct ecs_entity *ent) {
  struct ecs_entity *projectile = ecs_entity_new(firing_entity->position);
  Body *b = &ecs_add_component(projectile, ECS_COMPONENT_BODY)->body;
}
