#pragma once

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct point_ {
  int32_t x, y;
} point_t;

point_t *point_new(int32_t x, int32_t y);
void point_free(point_t *vec);
point_t *point_set(point_t *out, int32_t x, int32_t y);
point_t *point_copy(point_t *out, point_t *a);
point_t *point_add(point_t *out, point_t *a, point_t *b);
point_t *point_sub(point_t *out, point_t *a, point_t *b);
point_t *point_mul(point_t *out, point_t *a, point_t *b);
point_t *point_div(point_t *out, point_t *a, point_t *b);
point_t *point_scale(point_t *out, point_t *a, int32_t s);
float point_length(point_t *a);
int32_t point_length_squared(point_t *a);
point_t *point_normalize(point_t *out, point_t *a);
int32_t point_dot(point_t *a, point_t *b);
int32_t point_cross(point_t *a, point_t *b);
point_t *point_perp_left(point_t *out, point_t *a);
point_t *point_perp_right(point_t *out, point_t *a);
bool point_equal(const point_t *a, const point_t *b);
