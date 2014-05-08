#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <math.h>

/** \file geometry.h
  * \brief basic geometric structures and routines
**/

#define PI 3.14159265359
#define DEGTORAD(x) ((x)*(PI/180.0))
#define RADTODEG(x) ((x)/(PI*180.0))
#define ZEROVEC ((vector){.x = 0, .y = 0})

/** A 2D vector */
typedef struct vector {
  double x, y;
} vector;

/** A 2D point */
typedef struct point {
  int x, y;
} point;

/** A 2D rectangle */
typedef struct rectangle {
  int x, y;
  int w, h;
} rectangle;

/** linearly scale a vector by a factor */
vector vector_scale(vector v, double factor);
/** compute a vector sum (v1 + v2) */
vector vector_add(vector v1, vector v2);
/** compute a vector difference (v1 - v2) */
vector vector_sub(vector v1, vector v2);
/** negate a vector (-v) */
vector vector_neg(vector v);
/** magnitude of vector (|v|) */
double vector_len(vector v);
/** scale vector such that length = max_length */
vector vector_clamp(vector v, double max_length);
/** normalize a vector into a unit vector pointing in the same direction */
vector vector_norm(vector v);
/** return distance (absolute) between vectors */
double vector_dist(vector v1, vector v2);
/** return angle of vector (ragians) */
double vector_angle(vector v);

#endif /* end of include guard: GEOMETRY_H */
