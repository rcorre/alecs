#ifndef COLLIDER_SYS_H
#define COLLIDER_SYS_H

#include "ecs.h"

/** system to detect and handle collisions between \ref Collider components */
void collision_system_fn(double time);

/** create a hitrect the size of a sprite (takes scale into account) */
rectangle hitrect_from_sprite(sprite *sprite);

#endif /* end of include guard: COLLIDER_SYS_H */
