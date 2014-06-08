#ifndef ENEMIES_H
#define ENEMIES_H

#include "ecs.h"
#include "wave.h"
#include "system/weapon_sys.h"
#include "system/behavior_sys.h"

/** spawn an enemy
  * \param data struct containing info on how to spawn enemy
  * \return newly created entity
**/
void spawn_enemy(EnemySpawnData data);

/** spawn mine that seeks out the player
  * \param enter_from side of screen to enter from
  * \param player pointer to player entity
  * \return newly created entity
**/
ecs_entity* spawn_mine(Direction enter_from, ecs_entity *player);

#endif /* end of include guard: ENEMIES_H */
