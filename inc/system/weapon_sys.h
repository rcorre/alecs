#ifndef WEAPON_SYS_H
#define WEAPON_SYS_H

#include "ecs.h"

typedef struct Weapon {
  const char *name;
  const char *particle_effect;
  int max_lockons;
  double lockon_time;
  double initial_speed;
  double max_speed;
  double acceleration;
  double turn_rate;
  double power;
  double radius;
} Weapon;

/** set weapons for the weapon system */
void weapon_system_set_weapons(Weapon *primary, Weapon *secondary);

/** update the weapon system */
void weapon_system_fn(double time);

/** draw weapon targeting indicators */
void weapon_system_draw();

/** try locking on to the provided entity */
void weapon_set_target(struct ecs_entity *target);

/** clear lockon for entity if entity is the currently locked target */
void weapon_clear_target(struct ecs_entity *target);

/** fire the player's current weapon */
void weapon_fire_player(struct ecs_entity *player);

/** fire an enemy's weapon. player is type void* for use as a delegate arg */
void weapon_fire_enemy(struct ecs_entity *enemy, void *player);

/** switch between primary and secondary weapon */
void weapon_swap();

#endif /* end of include guard: WEAPON_SYS_H */
