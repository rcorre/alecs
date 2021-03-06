#ifndef SCENERY_H
#define SCENERY_H

#include "ecs.h"

/** system update function for scenery */
void scenery_system_fn(double time);

/** set frequency at which clouds spawn */
void scenery_sys_set_cloud_frequency(double clouds_per_sec);

/** fill out scenery before level begins. 
 *  Creates numerous entities, creating the appearance that the system has been
 *  run for awhile*/
void scenery_pre_populate();

/** add a scrolling background
  * \param name name of bitmap resource to use for background
  * \param depth layer at which to draw background
  * \param speed movement rate of background (towards \ref WEST)
  * \param offset starting x offset towards EAST
**/
void scenery_add_background(const char *name, int depth, double speed, int
    offset);

/** create an explosion
  * \param pos center position of explosion
  * \param size x and y scale of explosion
  * \param anim_rate rate of animation for explosion
  * \param tint shade of explosion
  * \param sound_name name of sound effect to play
**/
void scenery_make_explosion(vector pos, vector size, double anim_rate,
    ALLEGRO_COLOR tint, const char *sound_name);

#endif /* end of include guard: SCENERY_H */
