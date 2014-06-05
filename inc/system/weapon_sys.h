#ifndef WEAPON_SYS_H
#define WEAPON_SYS_H

#include "ecs.h"
#include "system/health_sys.h"
#include "system/scenery_sys.h"

typedef enum WeaponState {
  WEAPON_READY,
  WEAPON_FIRING,
  WEAPON_RELOADING
} WeaponState;

typedef struct Weapon {
  const char *name;
  const char *particle_effect;
  vector offset;
  int max_lockons;
  double lockon_time;
  double initial_speed;
  double max_speed;
  double acceleration;
  double turn_rate;
  double power;
  double radius;
  double fire_delay;
} Weapon;

/** set weapons for the weapon system */
void weapon_system_set_weapons(struct ecs_entity *player, Weapon *wep1, Weapon *wep2);

/** update the weapon system */
void weapon_system_fn(double time);

/** draw weapon targeting indicators */
void weapon_system_draw();

/** try locking on to the provided entity */
void weapon_set_target(struct ecs_entity *target);

/** clear lockon for entity if entity is the currently locked target */
void weapon_clear_target(struct ecs_entity *target);

/** fire the player's current weapon */
void weapon_fire_player();

/** fire an enemy's weapon. player is type void* for use as a delegate arg */
void weapon_fire_enemy(struct ecs_entity *enemy, void *player);

/** switch between primary and secondary weapon */
void weapon_swap();

/** switch between primary and secondary weapon */
void launch_flare(vector pos);

#endif /* end of include guard: WEAPON_SYS_H */
