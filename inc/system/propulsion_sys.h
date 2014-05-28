#ifndef PROPULSION_SYS_H
#define PROPULSION_SYS_H

#include "ecs.h"
#include "particle_effects.h"

void propulsion_system_fn(double time);

void propulsion_assign_sound(struct ecs_entity *prop_entity, char *sound_name);

#endif /* end of include guard: PROPULSION_SYS_H */
