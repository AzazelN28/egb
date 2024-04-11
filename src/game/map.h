#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../video/video.h"
#include "entity.h"

#define MAP_WIDTH 24
#define MAP_HEIGHT 24

#define MAP_VERSION 0x01
#define MAP_MAX_NAME_SIZE 32

typedef struct map_header_
{
  uint32_t version;
  uint8_t name[MAP_MAX_NAME_SIZE];
} map_header_t;

typedef struct map_flag_
{
  uint8_t id;
  uint8_t x, y;
  uint8_t type;
} map_flag_t;

typedef struct map_tiles_
{
  uint8_t width;
  uint8_t height;
  uint32_t *data;
} map_tiles_t;

typedef struct map_ {
  map_header_t header;
  map_tiles_t tiles;
  uint32_t num_flags;
  map_flag_t *flags;
} map_t;

extern uint32_t map[MAP_WIDTH][MAP_HEIGHT];

bool map_save_default();
bool map_save(const char* filename, map_t *map);
bool map_load(const char* filename, map_t *map);
void map_render();
