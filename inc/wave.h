#ifndef WAVE_H
#define WAVE_H

/** \file wave.h
  * \brief handles spawning waves of enemies
**/

#include "ecs.h"
#include "util/geometry.h"
#include "util/list.h"

/** data passed to enemy instance spawn functions */
typedef struct EnemySpawnData {
  vector start; ///< starting location of enemy
  vector target; ///< location to move to
  vector exit; ///< location to move to when exiting
  double duration; ///< how long to stay before exiting (seconds)
  ecs_entity *player; ///< pointer to player enemy
} EnemySpawnData;

/** function used by an \ref EnemyWave to spawn enemy instances.
 *  enemy starts at \c start and moves to \c target. It waits for \c duration
 *  before moving to \c exit. \c player is a pointer to the player entity */
typedef void (*wave_spawn_fn)(EnemySpawnData);

/** Represents a wave of enemies in a level */
typedef struct EnemyWave {
  wave_spawn_fn spawn_fn; ///< function called to spawn enemies
  int quantity;           ///< number of enemies to spawn
  Direction spawn_side;   ///< side of screen to spawn from
  Direction exit_side;    ///< side of screen to spawn from
  double start_time;      ///< number of seconds after which to start
  double spawn_delay;     ///< number of seconds between enemy spawns
  double duration;        ///< number of seconds enemies wait before leaving
} EnemyWave;

/** start spawning enemies */
void start_enemy_waves(struct ecs_entity *player);
/** update enemy spawns, returns \c false when all waves are expended */
bool update_enemy_waves(double time);
/** stop enemy wave spawning and free wave management structures */
void stop_enemy_waves();

#endif /* end of include guard: WAVE_H */
