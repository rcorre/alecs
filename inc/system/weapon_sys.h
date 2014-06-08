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

/// describes a weapon
typedef struct Weapon {
  const char *name;            ///< name of projectile sprite
  const char *particle_effect; ///< name of projectile particle effect
  vector offset;               ///< offset of launch site from entity
  vector initial_velocity;     ///< initial velocity of projectiles
  int max_lockons;             ///< maximum # of lockons for weapon
  double lockon_time;          ///< time to lock on to a target
  double max_speed;            ///< max speed of projectile
  double acceleration;         ///< acceleration of projectile
  double turn_rate;            ///< turn rate of projectile
  double power;                ///< damage dealt by projectile
  double radius;               ///< size of projectile explosion
  double deceleration_factor;  ///< deceleration_factor of projectile \c Body
  double fire_delay;           ///< time between successive launches
  ecs_entity_trigger fire_fn;  ///< special function to use when firing
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

void fire_swarmer_pod(struct ecs_entity *firing_entity);

/** fire an enemy's weapon. player is type void* for use as a delegate arg */
void weapon_fire_enemy(struct ecs_entity *enemy, void *player);

/** switch between primary and secondary weapon */
void weapon_swap();

/** switch between primary and secondary weapon */
void launch_flare(vector pos);

#endif /* end of include guard: WEAPON_SYS_H */
