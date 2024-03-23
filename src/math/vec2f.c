#include "vec2f.h"

vec2f_t *vec2f_new(float x, float y) {
  vec2f_t *out = calloc(1, sizeof(vec2f_t));
  assert(out != NULL);
  return vec2f_set(out, x, y);
}

void vec2f_free(vec2f_t *vec) {
  assert(vec != NULL);
  free(vec);
}

vec2f_t *vec2f_set(vec2f_t *out, float x, float y) {
  out->x = x;
  out->y = y;
  return out;
}

vec2f_t *vec2f_add(vec2f_t *out, vec2f_t *a, vec2f_t *b) {
  return vec2f_set(out, a->x + b->x, a->y + b->y);
}

vec2f_t *vec2f_sub(vec2f_t *out, vec2f_t *a, vec2f_t *b) {
  return vec2f_set(out, a->x - b->x, a->y - b->y);
}

vec2f_t *vec2f_mul(vec2f_t *out, vec2f_t *a, vec2f_t *b) {
  return vec2f_set(out, a->x * b->x, a->y * b->y);
}

vec2f_t *vec2f_div(vec2f_t *out, vec2f_t *a, vec2f_t *b) {
  return vec2f_set(out, a->x / b->x, a->y / b->y);
}

vec2f_t *vec2f_scale(vec2f_t *out, vec2f_t *a, float s) {
  return vec2f_set(out, a->x * s, a->y * s);
}

float vec2f_length(vec2f_t *a) { return sqrt(a->x * a->x + a->y * a->y); }

float vec2f_length_squared(vec2f_t *a) { return a->x * a->x + a->y * a->y; }

float vec2f_distance(vec2f_t *a, vec2f_t *b) {
  float dx = a->x - b->x;
  float dy = a->y - b->y;
  return sqrt(dx * dx + dy * dy);
}

vec2f_t *vec2f_normalize(vec2f_t *out, vec2f_t *a) {
  float len = vec2f_length(a);
  if (len == 0) {
    return vec2f_set(out, 0, 0);
  }
  return vec2f_scale(out, a, 1.0f / len);
}

float vec2f_dot(vec2f_t *a, vec2f_t *b) { return a->x * b->x + a->y * b->y; }

float vec2f_cross(vec2f_t *a, vec2f_t *b) { return a->x * b->y - a->y * b->x; }

vec2f_t *vec2f_perp_left(vec2f_t *out, vec2f_t *a) {
  return vec2f_set(out, -a->y, a->x);
}

vec2f_t *vec2f_perp_right(vec2f_t *out, vec2f_t *a) {
  return vec2f_set(out, a->y, -a->x);
}
