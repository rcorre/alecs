#ifndef HEALTH_SYS_H
#define HEALTH_SYS_H

#include "ecs.h"

void health_system_fn(double time);

/** deal damage to an entity */
void deal_damage(ecs_entity *entity, double amount);

#endif /* end of include guard: HEALTH_SYS_H */
