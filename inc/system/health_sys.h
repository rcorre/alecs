#ifndef HEALTH_SYS_H
#define HEALTH_SYS_H

#include "ecs.h"

void health_system_fn(double time);

/** deal damage to an entity */
void deal_damage(struct ecs_entity *entity, double amount);
/** create a health component
  * \param hp max and current health value
  * \param on_disable action to run when health hits 0
  * \param particle_effect_name name of particle effect to use
  * \return constructed Health struct
**/
Health make_health(double hp, ecs_entity_trigger on_disable, 
    char *particle_effect_name);

#endif /* end of include guard: HEALTH_SYS_H */
