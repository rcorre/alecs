#include "util/al_helper.h"

int randi(int min, int max) {
  return min + (rand() % (max + 1));
}

double randd(double min, double max) {
  double factor = (double)rand()/(double)RAND_MAX;
  return min + (max - min) * factor;
}

vector rand_unit_vec(double angle1, double angle2) {
  double angle = randd(angle1, angle2);
  vector v = {.x = cos(angle), .y = sin(angle)};
  return v;
}

vector rand_vec(double angle1, double angle2, double len_min, double len_max) {
  double len = randd(len_min, len_max);
  vector v = rand_unit_vec(angle1, angle2);
  return vector_scale(v, len);
}

vector rand_point(rectangle rect) {
  return (vector) { 
    randd(rect.x, rect.x + rect.w), randd(rect.x, rect.y + rect.h)
  };
}

double clamp(double v, double min, double max) {
  return fmax(fmin(v, max), min);
}

double lerp(double v1, double v2, double factor) {
  factor = clamp(factor, 0, 1);
  return v1 + (v2 - v1) * factor;
}

ALLEGRO_COLOR lerp_color(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2, double factor) {
  return al_map_rgba_f(
      lerp(c1.r, c2.r, factor),
      lerp(c1.g, c2.g, factor),
      lerp(c1.b, c2.b, factor),
      lerp(c1.a, c2.a, factor)
  );
}
