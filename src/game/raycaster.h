#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../video/video.h"
#include "../math/fixed.h"
#include "../math/vec2.h"
#include "../game/map.h"

#define RAYCASTER_TEXTURE_SIZE 64
#define RAYCASTER_TEXTURE_HALF_SIZE 32
#define RAYCASTER_TEXTURE_BUFFER_SIZE 4096
#define RAYCASTER_TEXTURE_FIXED_SIZE 0x400000
#define RAYCASTER_TEXTURE_FIXED_HALF_SIZE 0x200000

#define RAYCASTER_VIDEO_FIXED_WIDTH 0x1400000
#define RAYCASTER_VIDEO_FIXED_HEIGHT 0xC80000
#define RAYCASTER_VIDEO_FIXED_HALF_WIDTH 0xA00000
#define RAYCASTER_VIDEO_FIXED_HALF_HEIGHT 0x640000

#define RAYCASTER_MIN_Y 40
#define RAYCASTER_MAX_Y 160
#define RAYCASTER_MAX_DRAWING_Z 0x80000
#define RAYCASTER_MAX_ITERATIONS 32

#define RAYCASTER_NEG_ONE 0xFFFF0000
#define RAYCASTER_POS_ONE 0x00010000

typedef struct view_
{
  bool floor_ceil_hidden;
  bool changed_position;
  bool changed_rotation;
  vec2fix_t position;
  vec2fix_t direction;
  vec2fix_t plane;
} view_t;

typedef struct column_
{
  uint8_t side;
  uint8_t x;
  uint8_t y;
  fixed_t z;
  uint8_t u;
  uint8_t v;
  fixed_t inc_y;
  fixed_t tex_x;
  fixed_t tex_y;
  fixed_t fix_height;
  int16_t height;
  int16_t half_height;
  int16_t draw_start;
  int16_t draw_end;
} column_t;

typedef struct rows_
{
  vec2fix_t start; // start
  vec2fix_t end; // end
  vec2fix_t delta; // delta
  vec2fix_t step;
  vec2fix_t mult_step;
} rows_t;

typedef struct ray_
{
  bool hit;
  int16_t iterations;
  uint8_t side;
  fixed_t x;
  fixed_t wall_x;
  fixed_t tex_x;
  fixed_t perp_wall_dist;
  vec2fix_t tile;
  vec2int_t tilei;
  vec2fix_t step;
  vec2fix_t side_dist;
  vec2fix_t tile_delta;
  vec2fix_t direction;
  vec2fix_t delta_dist;
} ray_t;

typedef struct sprite_ {
  vec2fix_t position;
} sprite_t;

extern view_t view;
extern column_t columns[VIDEO_WIDTH];

void raycaster_start();
void raycaster_render();
