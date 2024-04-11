#pragma once

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

typedef struct rect_ {
  point_t position, size;
} rect_t;

rect_t *rect_new(int32_t x, int32_t y, int32_t w, int32_t h);
void rect_free(rect_t *rect);
rect_t *rect_set(rect_t *r, int32_t x, int32_t y, int32_t w, int32_t h);
rect_t *rect_copy(rect_t *out, rect_t *r);
bool rect_contains(rect_t *r, int32_t x, int32_t y);
bool rect_contains_point(rect_t *r, point_t *v);
bool rect_intersects(rect_t *a, rect_t *b);
rect_t *rect_intersection(rect_t *out, rect_t *a, rect_t *b);
