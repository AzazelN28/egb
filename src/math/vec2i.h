#pragma once

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct vec2i_ {
  int32_t x, y;
} vec2i_t;

vec2i_t *vec2i_new(int32_t x, int32_t y);
void vec2i_free(vec2i_t *vec);
vec2i_t *vec2i_set(vec2i_t *out, int32_t x, int32_t y);
vec2i_t *vec2i_copy(vec2i_t *out, vec2i_t *a);
vec2i_t *vec2i_add(vec2i_t *out, vec2i_t *a, vec2i_t *b);
vec2i_t *vec2i_sub(vec2i_t *out, vec2i_t *a, vec2i_t *b);
vec2i_t *vec2i_mul(vec2i_t *out, vec2i_t *a, vec2i_t *b);
vec2i_t *vec2i_div(vec2i_t *out, vec2i_t *a, vec2i_t *b);
vec2i_t *vec2i_scale(vec2i_t *out, vec2i_t *a, int32_t s);
float vec2i_length(vec2i_t *a);
int32_t vec2i_length_squared(vec2i_t *a);
vec2i_t *vec2i_normalize(vec2i_t *out, vec2i_t *a);
int32_t vec2i_dot(vec2i_t *a, vec2i_t *b);
int32_t vec2i_cross(vec2i_t *a, vec2i_t *b);
vec2i_t *vec2i_perp_left(vec2i_t *out, vec2i_t *a);
vec2i_t *vec2i_perp_right(vec2i_t *out, vec2i_t *a);
bool vec2i_equal(vec2i_t *a, vec2i_t *b);
