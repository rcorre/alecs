#ifndef SCENERY_H
#define SCENERY_H

#include "ecs.h"

/** system update function for scenery */
void scenery_system_fn(double time);

/** set frequency at which clouds spawn */
void scenery_sys_set_cloud_frequency(double clouds_per_sec);

#endif /* end of include guard: SCENERY_H */
