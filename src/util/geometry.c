#include "util/geometry.h"

vector vector_scale(vector vec, double factor) {
  vec.x *= factor;
  vec.y *= factor;
  return vec;
}

vector vector_add(vector v1, vector v2) {
  vector v = {.x = v1.x + v2.x, .y = v1.y + v2.y};
  return v;
}

vector vector_sub(vector v1, vector v2) {
  vector v = {.x = v1.x - v2.x, .y = v1.y - v2.y};
  return v;
}

vector vector_neg(vector v) {
  return (vector){.x = -v.x, .y = -v.y};
}

double vector_len(vector v) {
  return pow(pow(v.x, 2) + pow(v.y, 2), 0.5);
}

vector vector_clamp(vector v, double max_length) {
  double len = vector_len(v);
  return (len > max_length) ? vector_scale(v, 1 / len) : v;
}

vector vector_norm(vector v) {
  double len = vector_len(v);
  return len == 0 ? ZEROVEC : vector_scale(v, 1 / len);
}

double vector_dist(vector v1, vector v2) {
  return vector_len(vector_sub(v1, v2));
}

double vector_angle(vector v) {
  return atan2(v.y, v.x);
}

vector vector_rotate(vector v, double angle) {
  return (vector) {
    .x = v.x * cos(angle) - v.y * sin(angle),
    .y = v.y * cos(angle) + v.x * sin(angle)
  };
}

bool rect_contains_point(rectangle r, point p) {
  return 
    (r.x <= p.x) && (p.x <= r.x + r.w) && (r.y <= p.y) && (p.y <= r.y + r.h);
}

bool rect_intersect(rectangle r1, rectangle r2) {
  point topleft   = { r1.x, r1.y };
  point topright  = { r1.x + r1.w, r1.y };
  point botleft   = { r1.x, r1.y + r1.h };
  point botright  = { r1.x + r1.w, r1.y + r1.h };
  point r2topleft = { r2.x, r2.y };
  return (rect_contains_point(r2, topleft)  ||
          rect_contains_point(r2, botleft)  ||
          rect_contains_point(r2, botright) ||
          rect_contains_point(r2, topright) ||
          rect_contains_point(r1, r2topleft));
}

double normalize_angle(double angle) {
  while (angle < 0) { angle += 2 * PI; }
  while (angle >= 2 * PI) { angle -= 2 * PI; }
  return (angle > PI) ? (angle - 2 * PI) : angle;
}

double angle_between(double from, double to) {
  return normalize_angle(normalize_angle(to) - normalize_angle(from));
}
