#ifndef WEAPON_SYS_H
#define WEAPON_SYS_H

#include "ecs.h"

/** update the weapon system */
void weapon_system_fn(double time);

/** try locking on to the provided entity */
void weapon_set_target(ecs_entity *target);

#endif /* end of include guard: WEAPON_SYS_H */
