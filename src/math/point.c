#include "point.h"

point_t *point_new(int32_t x, int32_t y) {
  point_t *vec = calloc(1, sizeof(point_t));
  assert(vec != NULL);
  return point_set(vec, x, y);
}

void point_free(point_t *vec) {
  assert(vec != NULL);
  free(vec);
}

point_t *point_set(point_t *out, int32_t x, int32_t y) {
  out->x = x;
  out->y = y;
  return out;
}

point_t *point_copy(point_t *out, point_t *a) {
  return point_set(out, a->x, a->y);
}

point_t *point_add(point_t *out, point_t *a, point_t *b) {
  return point_set(out, a->x + b->x, a->y + b->y);
}

point_t *point_sub(point_t *out, point_t *a, point_t *b) {
  return point_set(out, a->x - b->x, a->y - b->y);
}

point_t *point_mul(point_t *out, point_t *a, point_t *b) {
  return point_set(out, a->x * b->x, a->y * b->y);
}

point_t *point_div(point_t *out, point_t *a, point_t *b) {
  return point_set(out, a->x / b->x, a->y / b->y);
}

point_t *point_scale(point_t *out, point_t *a, int32_t s) {
  return point_set(out, a->x * s, a->y * s);
}

float point_length(point_t *a) { return sqrt(a->x * a->x + a->y * a->y); }

int32_t point_length_squared(point_t *a) { return a->x * a->x + a->y * a->y; }

float point_distance(point_t *a, point_t *b) {
  int32_t dx = a->x - b->x;
  int32_t dy = a->y - b->y;
  return sqrt(dx * dx + dy * dy);
}

point_t *point_normalize(point_t *out, point_t *a) {
  int32_t len = point_length(a);
  if (len == 0) {
    return point_set(out, 0, 0);
  }
  return point_scale(out, a, 1.0f / len);
}

int32_t point_dot(point_t *a, point_t *b) { return a->x * b->x + a->y * b->y; }

int32_t point_cross(point_t *a, point_t *b) {
  return a->x * b->y - a->y * b->x;
}

point_t *point_perp_left(point_t *out, point_t *a) {
  return point_set(out, -a->y, a->x);
}

point_t *point_perp_right(point_t *out, point_t *a) {
  return point_set(out, a->y, -a->x);
}

bool point_equal(const point_t *a, const point_t *b) {
  return a->x == b->x && a->y == b->y;
}
