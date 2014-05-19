#ifndef WEAPON_SYS_H
#define WEAPON_SYS_H

#include "ecs.h"

/** update the weapon system */
void weapon_system_fn(double time);

/** draw weapon targeting indicators */
void weapon_system_draw();

/** try locking on to the provided entity */
void weapon_set_target(struct ecs_entity *target);

/** clear lockon for entity if entity is the currently locked target */
void weapon_clear_target(struct ecs_entity *target);

/** fire the current weapon */
void weapon_fire();

#endif /* end of include guard: WEAPON_SYS_H */
