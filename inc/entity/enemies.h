#ifndef ENEMIES_H
#define ENEMIES_H

#include "ecs.h"
#include "system/weapon_sys.h"
#include "system/behavior_sys.h"

/** spawn an enemy
  * \param pos position to move to
  * \param enter_from side of screen to enter from
  * \param player pointer to player entity
  * \return newly created entity
**/
ecs_entity* spawn_enemy(vector pos, Direction enter_from, ecs_entity *player);

#endif /* end of include guard: ENEMIES_H */
