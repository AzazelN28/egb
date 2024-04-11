#include "entity.h"

entity_t player = {
    {FIXED_FROM_INT(22), FIXED_FROM_INT(10)},
    {FIXED_FROM_INT(22), FIXED_FROM_INT(10)},
    {FIXED_FROM_INT(-1), FIXED_FROM_INT(0)},
    0, 0};

bool entity_try_move(entity_t *entity)
{
  // 0x3fff == 0.25
  // 0x7fff == 0.5
  // 0xffff == 0.9999
  int8_t minx = FIXED_TO_INT(entity->position.x - 0x3fff);
  int8_t miny = FIXED_TO_INT(entity->position.y - 0x3fff);
  int8_t maxx = FIXED_TO_INT(entity->position.x + 0x3fff);
  int8_t maxy = FIXED_TO_INT(entity->position.y + 0x3fff);

  for (int8_t y = miny; y <= maxy; y++)
  {
    for (int8_t x = minx; x <= maxx; x++)
    {
      if (map[x][y] != 0)
      {
        return false;
      }
    }
  }
  return true;
}

void entity_move(entity_t *entity)
{
  entity->collision = COLLISION_NONE;

  vec2fix_t current_position = {
      .x = entity->position.x,
      .y = entity->position.y};

  entity->position.x = entity->next_position.x;
  entity->position.y = entity->next_position.y;
  if (entity_try_move(entity))
  {
    return;
  }

  entity->collision = COLLISION_Y;
  entity->position.x = entity->next_position.x;
  entity->position.y = current_position.y;
  if (entity_try_move(entity))
  {
    return;
  }

  entity->collision = COLLISION_X;
  entity->position.x = current_position.x;
  entity->position.y = entity->next_position.y;
  if (entity_try_move(entity))
  {
    return;
  }

  entity->collision = COLLISION_XY;
  entity->position.x = current_position.x;
  entity->position.y = current_position.y;
}
