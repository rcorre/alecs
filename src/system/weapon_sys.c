#include "system/weapon_sys.h"
#include "system/health_sys.h"
#include "system/scenery_sys.h"

// lockon constants
static const float indicator_radius = 18;
static const float indicator_thickness = 5;
// grace period after launching during which a projectile cannot hit friendlies
static const double friendly_fire_time = 1;
#define PRIMARY_LOCK_COLOR al_map_rgba(128, 0, 0, 200)
#define SECONDARY_LOCK_COLOR al_map_rgba(0, 0, 128, 128)

// explosion constants
static const double explosion_animate_rate = 50; // frames/sec

static struct ecs_entity *current_target;
static double current_lockon_time;
static list *lockon_list;
static Weapon *current_weapon, *alternate_weapon;

static void fire_at_target(struct ecs_entity *fired_by,
    struct ecs_entity *target, ecs_entity_team team);
static void draw_lockon(struct ecs_entity *target);
// collision handler for projectile
static void hit_target(struct ecs_entity *projectile, struct ecs_entity *target);
// blow up a projectile
static void explode(struct ecs_entity *projectile);
// timer trigger to swich projectile team to neutral
static void friendly_fire_timer_fn(struct ecs_entity *projectile);

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
  vector fire_pos = vector_add(firing_entity->position, current_weapon->offset);
  struct ecs_entity *projectile = ecs_entity_new(firing_entity->position);
  projectile->position = fire_pos;
  projectile->angle = -PI / 2;
  ecs_attach_sprite(projectile, "seeker", 0);
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
  collider->on_collision = hit_target;
  projectile->team = team;
  Timer *timer = &ecs_add_component(projectile, ECS_COMPONENT_TIMER)->timer;
  timer->time_left = friendly_fire_time;
  timer->timer_action = friendly_fire_timer_fn;
  // mouse listener (for weapon lockon)
  MouseListener *listener =
    &ecs_add_component(projectile, ECS_COMPONENT_MOUSE_LISTENER)->mouse_listener;
  listener->click_rect = collider->rect;
  listener->on_enter = weapon_set_target;
  listener->on_leave = weapon_clear_target;
  // make small explosion for launch
  scenery_make_explosion(fire_pos, (vector){1,2}, 50, al_map_rgb_f(1,1,1), "launch");
}

static void draw_lockon(struct ecs_entity *target) {
  al_draw_arc(target->position.x, target->position.y,
      indicator_radius, 0, 2 * PI, PRIMARY_LOCK_COLOR,
      indicator_thickness);
}

static void hit_target(struct ecs_entity *projectile, struct ecs_entity *target)
{
  deal_damage(target, 10);
  explode(projectile);
}

static void explode(struct ecs_entity *projectile) {
  scenery_make_explosion(projectile->position, (vector){3,3},
      explosion_animate_rate, al_map_rgb(255,255,255), "explosion1");
  ecs_entity_free(projectile);
}

static void friendly_fire_timer_fn(struct ecs_entity *projectile) {
  projectile->team = TEAM_NEUTRAL;
  Timer *t = &projectile->components[ECS_COMPONENT_TIMER]->timer;
  t->time_left = 5.0;  // TODO: use projectile duration time
  t->timer_action = explode;
}

void launch_flare(vector pos) {
  struct ecs_entity *flare = ecs_entity_new(pos);
  flare->angle = -PI / 2;
  /*
  sprite *s = ecs_attach_sprite(flare, "explosion", 3);
  s->frame_width = 64;
  s->frame_height = 64;
  s->scale = (vector){0.5, 0.5};
  s->tint = al_map_rgb_f(1,0,0);
  s->
  */
  Body *b = &ecs_add_component(flare, ECS_COMPONENT_BODY)->body;
  b->max_linear_velocity = 600;
  b->velocity = (vector){-50, -500};
  b->destroy_on_exit = NONE;
  Propulsion *p =
    &ecs_add_component(flare, ECS_COMPONENT_PROPULSION)->propulsion;
  p->linear_accel = 400;
  p->linear_throttle = (vector){-1, 0};
  p->turn_rate = 0;
  p->particle_effect =
    get_particle_generator("flare");
  p->directed = true;
  /*
  Collider *collider = &ecs_add_component(projectile,
      ECS_COMPONENT_COLLIDER)->collider;
  collider->rect = hitrect_from_sprite(projectile->sprite);
  collider->on_collision = hit_target;
  projectile->team = team;
  */
  // timer to destroy flare after 6 seconds
  Timer *t = &ecs_add_component(flare, ECS_COMPONENT_TIMER)->timer;
  t->time_left = 6;
  t->timer_action = ecs_entity_free;
  // make small explosion for launch
  scenery_make_explosion(pos, (vector){1,2}, 50, al_map_rgb_f(1,0,0), "launch");
}
