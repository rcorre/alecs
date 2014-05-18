#ifndef UTIL_H
#define UTIL_H

/** \file al_helper.h
  * \brief some helper methods for working in allegro
**/

#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/color.h>
#include "util/geometry.h"

/**return a random int between min and max (inclusive) */
int randi(int min, int max);

/**return a random double between min and max */
double randd(double min, double max);

/**return a unit vector with angle between angle1 and angle2 (radians) */
vector rand_unit_vec(double angle1, double angle2);

/** return a vector with angle between angle1 and angle2 (radians)
 ** and length between len_min and len_max
 */
vector rand_vec(double angle1, double angle2, double len_min, double len_max);

/** return a vector within the bounds of rect */
vector rand_point(rectangle rect);

/** limit v to fall between between min and max */
double clamp(double v, double min, double max);

/** return a value linearly interpolated between v1 and v2
 ** where factor is between 0 and 1
 */
double lerp(double v1, double v2, double factor);

/** return a color linearly interpolated between two colors */
// where factor is a value between 0 (c1) and 1.0 (c2)
ALLEGRO_COLOR lerp_color(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2, double factor);

#endif /* end of include guard: UTIL_H */
