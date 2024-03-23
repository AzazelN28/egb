#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../math/point.h"

enum direction {
  DIR_EAST = 0,
  DIR_SOUTH,
  DIR_WEST,
  DIR_NORTH,
};

typedef struct transform_ {
  point_t position;
  uint8_t direction;
} transform_t;

bool transform_are_adjacent(const transform_t* a, const transform_t* b);
bool transform_are_facing(const transform_t* a, const transform_t* b);
