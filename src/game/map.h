#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../video/video.h"
#include "entity.h"

#define MAP_WIDTH 24
#define MAP_HEIGHT 24

#define MAP_SIGNATURE 0x4d415030
#define MAP_MAX_NAME_SIZE 32

typedef uint8_t map_size_t;
typedef uint32_t map_tile_t;

typedef struct map_header_
{
  uint32_t signature;
  uint8_t name[MAP_MAX_NAME_SIZE];
} map_header_t;

typedef struct map_flag_
{
  uint8_t id;
  uint8_t type;
  map_size_t x, y;
} map_flag_t;

typedef struct map_tiles_
{
  map_size_t width;
  map_size_t height;
  map_tile_t data[MAP_WIDTH][MAP_HEIGHT];
} map_tiles_t;

typedef struct map_ {
  map_header_t header;
  map_tiles_t tiles;
  uint32_t num_flags;
  map_flag_t *flags;
} map_t;

extern map_t map;
// extern uint32_t map[MAP_WIDTH][MAP_HEIGHT];

bool map_save_current(const char* filename);
bool map_load_current(const char* filename);
bool map_save(const char* filename, map_t *map);
bool map_load(const char* filename, map_t *map);
void map_render();
