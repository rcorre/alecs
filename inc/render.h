#ifndef RENDER_H
#define RENDER_H

/** \file render.h
  * \brief structs and functions for rendering images to the display
**/

#include "al_game.h"
#include "util/geometry.h"
#include "util/list.h"

/** number of sprite layers above and below 0. */
#define SPRITE_LAYER_LIMIT 5
#define SPRITE_LAYER_COUNT (2 * SPRITE_LAYER_LIMIT + 1)

typedef enum AnimationType {
  ANIMATE_OFF,
  ANIMATE_ONCE,
  ANIMATE_LOOP
} AnimationType;

/** \ref component that draws a bitmap to the screen */
typedef struct sprite {
  /** source bitmap used to draw sprite */
  ALLEGRO_BITMAP *bitmap;
  /** color to shade drawn sprite with */
  ALLEGRO_COLOR tint;
  /** center of bitmap */
  vector center;
  /** horizontal and vertical scaling factors to use when drawing sprite */
  vector scale;
  /** reference to owner's position */
  vector *position_ptr;
  /** reference to owner's rotation */
  double *angle_ptr;
  /** layer at which to draw sprite - modify only using \ref sprite_set_depth */
  int _depth;
  /** node holding sprite in backing sprite store - DO NOT MODIFY */
  list_node *_node;
  int frame_width;       ///< width of a single frame within the spritesheet
  int frame_height;      ///< width of a single frame within the spritesheet
  double animation_rate; ///< frames/sec
  double _animation_timer; ///< countdown till next frame
  AnimationType animation_type; ///< how to animate sprite (if at all)
  bool destroy_on_animation_end; //< if true, destroy entity when animation done
  int current_frame;  ///< frame currently being displayed
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

/** delete a sprite*/
void sprite_free(sprite *sprite);

/** change the depth of a sprite */
void sprite_set_depth(sprite *sprite, int depth);

/** draw every sprite to the display */
void render_all_sprites();

/** return the width of a sprite (taking scaling into account) */
int sprite_width(sprite *s);

/** return the height of a sprite (taking scaling into account) */
int sprite_height(sprite *s);

/** return the number of animation frames for a sprite */
int sprite_num_frames(sprite *s);

#endif /* end of include guard: RENDER_H */
