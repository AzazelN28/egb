#pragma once

#include "../math/fixed.h"
#include "../math/vec2.h"
#include "map.h"

#define MAP_WIDTH 24
#define MAP_HEIGHT 24

#define COLLISION_NONE    0x00
#define COLLISION_X  0x01
#define COLLISION_Y  0x02
#define COLLISION_XY 0x03
#define COLLISION_ENTITY  0x10

typedef struct entity_
{
  fixed_t radius;
  vec2fix_t position;
  vec2fix_t next_position;
  vec2fix_t direction;
  fixang_t rotation;
  uint8_t collision;
  vec2int8_t tile;

  // necesario para pintarlos en orden y saber si son visibles,
  // además de para usar el z-buffer.
  fixed_t z; //
  fixed_t x;
  fixed_t y;

  // siguiente visible.
  struct entity_ *next_visible;
  struct entity_ *next_adjacent;
} entity_t;

extern entity_t player;
extern entity_t other;

extern entity_t *adjacent_entities[MAP_WIDTH][MAP_HEIGHT];
extern entity_t *visible_entities;
extern uint8_t num_visible_entities;

bool entity_try_move(entity_t *entity);
void entity_move(entity_t *entity);
void entity_update(entity_t *entity);
void entity_clear_visible();
void entity_add_visible(entity_t *entity);
void entity_clear_adjacent();
void entity_add_adjacent(entity_t *entity);
