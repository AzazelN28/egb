#pragma once

#include <assert.h>
#include <math.h>
#include <stdlib.h>

typedef struct vec2f_ {
  float x, y;
} vec2f_t;

vec2f_t *vec2f_new(float x, float y);
void vec2f_free(vec2f_t *vec);
vec2f_t *vec2f_set(vec2f_t *out, float x, float y);
vec2f_t *vec2f_add(vec2f_t *out, vec2f_t *a, vec2f_t *b);
vec2f_t *vec2f_sub(vec2f_t *out, vec2f_t *a, vec2f_t *b);
vec2f_t *vec2f_mul(vec2f_t *out, vec2f_t *a, vec2f_t *b);
vec2f_t *vec2f_div(vec2f_t *out, vec2f_t *a, vec2f_t *b);
vec2f_t *vec2f_scale(vec2f_t *out, vec2f_t *a, float s);
float vec2f_length(vec2f_t *a);
float vec2f_length_squared(vec2f_t *a);
vec2f_t *vec2f_normalize(vec2f_t *out, vec2f_t *a);
float vec2f_dot(vec2f_t *a, vec2f_t *b);
float vec2f_cross(vec2f_t *a, vec2f_t *b);
vec2f_t *vec2f_perp_left(vec2f_t *out, vec2f_t *a);
vec2f_t *vec2f_perp_right(vec2f_t *out, vec2f_t *a);
