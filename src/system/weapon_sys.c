#include "system/weapon_sys.h"

// lockon constants
static const float indicator_radius = 18;
static const float indicator_thickness = 5;
#define PRIMARY_LOCK_COLOR al_map_rgba(128, 0, 0, 200)
#define SECONDARY_LOCK_COLOR al_map_rgba(0, 0, 128, 128)

// explosion constants
static const double explosion_animate_rate = 25; // frames/sec

static struct ecs_entity *current_target;
static double current_lockon_time;
static list *lockon_list;
static Weapon *current_weapon, *alternate_weapon;

static void fire_at_target(struct ecs_entity *fired_by,
    struct ecs_entity *target, ecs_entity_team team);
static void draw_lockon(struct ecs_entity *target);
static void explode(struct ecs_entity *projectile, void *weapon_data);

void weapon_system_fn(double time) {
  if (current_target) {
    current_lockon_time += time;
    if (current_lockon_time > current_weapon->lockon_time) {
      list_push(lockon_list, current_target);
      weapon_clear_target(current_target);
    }
  }
}

void weapon_system_draw() {
  if (current_target) {
    al_draw_arc(current_target->position.x, current_target->position.y,
        indicator_radius, 0,
        2 * PI * current_lockon_time / current_weapon->lockon_time,
        PRIMARY_LOCK_COLOR, indicator_thickness);
  }
  if (lockon_list) {
    list_each(lockon_list, (list_lambda)draw_lockon);
  }
}

void weapon_set_target(struct ecs_entity *target) {
  if (current_target == NULL) {
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

void weapon_fire_player(struct ecs_entity *fired_by) {
  // fire at each target, then remove it from the list
  for (list_node *node = lockon_list->head; node; node = node->next) {
    ecs_entity *target = (ecs_entity*)node->value;
    fire_at_target(fired_by, target, TEAM_FRIENDLY);
  }
  list_clear(lockon_list, NULL);
}

void weapon_fire_enemy(struct ecs_entity *enemy, void *player) {
  fire_at_target(enemy, (ecs_entity*)player, TEAM_ENEMY);
}

void weapon_swap() {
  Weapon *temp = current_weapon;
  current_weapon = alternate_weapon;
  alternate_weapon = temp;
  list_clear(lockon_list, NULL);
  weapon_clear_target(current_target);
}

static void fire_at_target(struct ecs_entity *firing_entity,
    struct ecs_entity *target, ecs_entity_team team)
{
  struct ecs_entity *projectile = ecs_entity_new(firing_entity->position);
  projectile->position = firing_entity->position;
  projectile->angle = -PI / 2;
  ecs_attach_sprite(projectile, "seeker", -1);
  Body *b = &ecs_add_component(projectile, ECS_COMPONENT_BODY)->body;
  b->max_linear_velocity = current_weapon->max_speed;
  Propulsion *p =
    &ecs_add_component(projectile, ECS_COMPONENT_PROPULSION)->propulsion;
  p->linear_accel = current_weapon->acceleration;
  p->turn_rate = current_weapon->turn_rate;
  p->particle_effect =
    get_particle_generator((char*)current_weapon->particle_effect);
  p->directed = true;
  Behavior *behavior = &ecs_add_component(projectile,
      ECS_COMPONENT_BEHAVIOR)->behavior;
  behavior->target = target;
  behavior->type = BEHAVIOR_FOLLOW;
  Collider *collider = &ecs_add_component(projectile,
      ECS_COMPONENT_COLLIDER)->collider;
  collider->rect = hitrect_from_sprite(projectile->sprite);
  collider->on_collision = (ecs_entity_delegate) {
    .delegate = explode, .data = current_weapon
  };
  projectile->team = team;
}

static void draw_lockon(struct ecs_entity *target) {
  al_draw_arc(target->position.x, target->position.y,
      indicator_radius, 0, 2 * PI, PRIMARY_LOCK_COLOR,
      indicator_thickness);
}

static void explode(struct ecs_entity *projectile, void *weapon_data) {
  ecs_entity *boom = ecs_entity_new(projectile->position);
  sprite *anim = ecs_attach_animation(boom, "explosion", 1, 32, 32,
      explosion_animate_rate, ANIMATE_ONCE);
  anim->scale = (vector){3,3};
  ecs_entity_free(projectile);
}
