#pragma once

#include "../math/fixed.h"
#include "../math/vec2.h"
#include "map.h"

#define COLLISION_NONE    0x00
#define COLLISION_X  0x01
#define COLLISION_Y  0x02
#define COLLISION_XY 0x03
#define COLLISION_ENTITY  0x10

typedef struct entity_
{
  vec2fix_t position;
  vec2fix_t next_position;
  vec2fix_t direction;
  fixang_t rotation;
  uint8_t collision;
} entity_t;

extern entity_t player;

bool entity_try_move(entity_t *entity);
void entity_move(entity_t *entity);
