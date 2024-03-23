#include "vec2i.h"

vec2i_t *vec2i_new(int32_t x, int32_t y) {
  vec2i_t *vec = calloc(1, sizeof(vec2i_t));
  assert(vec != NULL);
  return vec2i_set(vec, x, y);
}

void vec2i_free(vec2i_t *vec) {
  assert(vec != NULL);
  free(vec);
}

vec2i_t *vec2i_set(vec2i_t *out, int32_t x, int32_t y) {
  out->x = x;
  out->y = y;
  return out;
}

vec2i_t *vec2i_copy(vec2i_t *out, vec2i_t *a) {
  return vec2i_set(out, a->x, a->y);
}

vec2i_t *vec2i_add(vec2i_t *out, vec2i_t *a, vec2i_t *b) {
  return vec2i_set(out, a->x + b->x, a->y + b->y);
}

vec2i_t *vec2i_sub(vec2i_t *out, vec2i_t *a, vec2i_t *b) {
  return vec2i_set(out, a->x - b->x, a->y - b->y);
}

vec2i_t *vec2i_mul(vec2i_t *out, vec2i_t *a, vec2i_t *b) {
  return vec2i_set(out, a->x * b->x, a->y * b->y);
}

vec2i_t *vec2i_div(vec2i_t *out, vec2i_t *a, vec2i_t *b) {
  return vec2i_set(out, a->x / b->x, a->y / b->y);
}

vec2i_t *vec2i_scale(vec2i_t *out, vec2i_t *a, int32_t s) {
  return vec2i_set(out, a->x * s, a->y * s);
}

float vec2i_length(vec2i_t *a) { return sqrt(a->x * a->x + a->y * a->y); }

int32_t vec2i_length_squared(vec2i_t *a) { return a->x * a->x + a->y * a->y; }

float vec2i_distance(vec2i_t *a, vec2i_t *b) {
  int32_t dx = a->x - b->x;
  int32_t dy = a->y - b->y;
  return sqrt(dx * dx + dy * dy);
}

vec2i_t *vec2i_normalize(vec2i_t *out, vec2i_t *a) {
  int32_t len = vec2i_length(a);
  if (len == 0) {
    return vec2i_set(out, 0, 0);
  }
  return vec2i_scale(out, a, 1.0f / len);
}

int32_t vec2i_dot(vec2i_t *a, vec2i_t *b) { return a->x * b->x + a->y * b->y; }

int32_t vec2i_cross(vec2i_t *a, vec2i_t *b) {
  return a->x * b->y - a->y * b->x;
}

vec2i_t *vec2i_perp_left(vec2i_t *out, vec2i_t *a) {
  return vec2i_set(out, -a->y, a->x);
}

vec2i_t *vec2i_perp_right(vec2i_t *out, vec2i_t *a) {
  return vec2i_set(out, a->y, -a->x);
}

bool vec2i_equal(vec2i_t *a, vec2i_t *b) {
  return a->x == b->x && a->y == b->y;
}
