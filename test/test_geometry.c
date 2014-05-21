#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util/geometry.h"

static bool almost_equal(double d1, double d2) {
  return abs(d1 - d2) < 1E-6;
}

static bool vec_equal(vector v1, vector v2) {
  return almost_equal(v1.x, v2.x) && almost_equal(v1.y, v2.y);
}

int main(int argc, char *argv[]) {
  // add, subtract
  vector ux = {1, 0};
  vector uy = {0, 1};

  assert(vec_equal(vector_add(ux, uy), (vector){1,1}));
  assert(vec_equal(vector_add(uy, ux), (vector){1,1}));
  assert(vec_equal(vector_sub(ux, ux), (vector){0,0}));
  assert(vec_equal(vector_sub(ux, uy), (vector){1,-1}));
  assert(vec_equal(vector_sub(uy, ux), (vector){-1,1}));
  
  // scale, angle, negate, length
  vector v45 = {cos(PI/4), sin(PI/4)};
  vector v45neg = {-cos(PI/4), -sin(PI/4)};
  assert(almost_equal(vector_angle(v45), PI / 4));
  assert(almost_equal(vector_angle(v45neg), -3 * PI / 4));
  assert(almost_equal(vector_angle(ux), 0));
  assert(almost_equal(vector_angle(uy), PI / 2));
  assert(vec_equal(vector_neg(v45), v45neg));
  assert(vec_equal(vector_scale(v45, 5), (vector){v45.x * 5, v45.y * 5}));
  assert(vec_equal(vector_scale(v45, -1), v45neg));
  assert(almost_equal(vector_len(v45), 1));

  // rotation
  assert(vec_equal(vector_rotate(v45, PI), vector_neg(v45)));
  assert(vec_equal(vector_rotate(v45, -PI), vector_neg(v45)));
  assert(vec_equal(vector_rotate(v45, 2 * PI), v45));
  assert(vec_equal(vector_rotate(v45, -2 * PI), v45));
  assert(vec_equal(vector_rotate(v45, PI / 4), uy));
  assert(vec_equal(vector_rotate(v45, -PI / 4), ux));

  // normalize angle
  assert(almost_equal(normalize_angle(0)           , 0));
  assert(almost_equal(normalize_angle(2*PI)        , 0));
  assert(almost_equal(normalize_angle(-2*PI)       , 0));
  assert(almost_equal(normalize_angle(4*PI)        , 0));
  assert(almost_equal(normalize_angle(-4*PI)       , 0));
  assert(almost_equal(normalize_angle(-5*PI)       , -PI));
  assert(almost_equal(normalize_angle(-3*PI / 2)   , PI / 2));
  assert(almost_equal(normalize_angle(-PI / 6)     , -PI / 6));
  assert(almost_equal(normalize_angle(20 * PI / 3) , 2 * PI / 3));

  // scaling and rotating
  vector v30 = {cos(PI/6), sin(PI/6)};
  vector v5_30 = vector_scale(v30, 5);
  vector v5_60 = vector_rotate(v5_30, PI / 6);
  vector v5_neg60 = vector_rotate(v5_30, -PI / 2);
  assert(vec_equal(v5_30, (vector){5*cos(PI/6), 5*sin(PI/6)} ));
  assert(almost_equal(vector_len(v5_30), 5));
  assert(almost_equal(vector_len(v5_60), 5));
  assert(almost_equal(vector_len(v5_neg60), 5));
  assert(almost_equal(vector_angle(v5_60), PI/3));
  assert(almost_equal(vector_angle(v5_neg60), -PI/3));

  // angle between
  assert(almost_equal(angle_between(PI/6, PI/3)  , PI/6));
  assert(almost_equal(angle_between(PI/3, PI/6)  , -PI/6));
  assert(almost_equal(angle_between(PI/3, PI)    , 2*PI/ 3));
  assert(almost_equal(angle_between(PI/3, 5*PI/3), -2*PI/ 3));
  assert(almost_equal(angle_between(-PI, PI), 0));
  assert(almost_equal(angle_between(PI, -PI), 0));
  assert(almost_equal(angle_between(-2*PI/3, 2*PI/3), -2*PI/3));
}
