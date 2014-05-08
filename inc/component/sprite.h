#ifndef SPRITE_H
#define SPRITE_H

#include "ecs.h"
#include "al_game.h"

/** \ref component that draws a bitmap to the screen */
typedef struct sprite {
  /** source bitmap used to draw sprite */
  ALLEGRO_BITMAP *bitmap;
  /** color to shade drawn sprite with */
  ALLEGRO_COLOR tint;
  /** scaling factor to use when drawing sprite. 1.0 is unscaled */
  double scale;
} sprite;

/** \ref component_manager for components of type \ref sprite */
extern struct ecs_component_manager sprite_manager;

#endif /* end of include guard: SPRITE_H */
