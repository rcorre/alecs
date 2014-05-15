#ifndef RENDER_H
#define RENDER_H

/** \file render.h
  * \brief structs and functions for rendering images to the display
**/

#include "al_game.h"
#include "util/geometry.h"
#include "util/list.h"

/** number of sprite layers above and below 0.
 *  valid layers are -(SPRITE_LAYER_LIMIT - 1) to (SPRITE_LAYER_LIMIT - 1)*/
#define SPRITE_LAYER_LIMIT 5
#define SPRITE_LAYER_COUNT (2 * SPRITE_LAYER_LIMIT + 1)

/** \ref component that draws a bitmap to the screen */
typedef struct sprite {
  /** source bitmap used to draw sprite */
  ALLEGRO_BITMAP *bitmap;
  /** color to shade drawn sprite with */
  ALLEGRO_COLOR tint;
  /** center of bitmap */
  vector center;
  /** width of bitmap */
  int width;
  /** height of bitmap */
  int height;
  /** scaling factor to use when drawing sprite. 1.0 is unscaled */
  double scale;
  /** reference to owner's position */
  vector *position_ptr;
  /** reference to owner's rotation */
  double *angle_ptr;
} sprite;

/** initialize sprite framework */
void sprite_init();
/** shut down sprite framework */
void sprite_shutdown();
/** create a new sprite
  * \param name name of bitmap resource to load
  * \param ref_position pointer to position of owner
  * \param ref_angle pointer to rotation of owner
  * \param depth layer at which to draw sprite
  * \return return value description
**/
sprite* sprite_new(const char *name, vector *ref_position,
    double *ref_angle, int depth);
void sprite_free(sprite *sprite);
/** draw every sprite to the display */
void render_all_sprites();

#endif /* end of include guard: RENDER_H */
