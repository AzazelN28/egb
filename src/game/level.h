#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct level_tile_t {
  uint8_t type;
  uint8_t trigger;
  uint8_t flags;
  uint8_t data;
} level_tile_t;

typedef struct level_ {
  uint16_t width;
  uint16_t height;
  level_tile_t* data;
} level_t;

level_t* level_create(uint16_t width, uint16_t height);
void level_destroy(level_t* level);
