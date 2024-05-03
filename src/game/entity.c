#include "entity.h"

entity_t player = {
    FIXED_HALF_UNIT,
    {FIXED_FROM_INT(22), FIXED_FROM_INT(10)},
    {FIXED_FROM_INT(22), FIXED_FROM_INT(10)},
    {FIXED_NEG_ONE, 0},
    0,
    0,
    {22, 10},
    0,
    NULL,
    NULL
    };

entity_t other = {
    FIXED_HALF_UNIT,
    {FIXED_FROM_INT(22), FIXED_FROM_INT(10)},
    {FIXED_FROM_INT(22), FIXED_FROM_INT(10)},
    {FIXED_NEG_ONE, 0},
    0,
    0,
    {22, 10},
    0,
    NULL,
    NULL};

entity_t *adjacent_entities[MAP_WIDTH][MAP_HEIGHT] = {NULL};
entity_t *visible_entities = NULL;
uint8_t num_visible_entities = 0;

bool entity_try_move(entity_t *entity)
{
  // 0x3fff == 0.25
  // 0x7fff == 0.5
  // 0xffff == 0.9999
  int8_t minx = FIXED_TO_INT((entity->position.x - 0x3fff));
  int8_t miny = FIXED_TO_INT((entity->position.y - 0x3fff));
  int8_t maxx = FIXED_TO_INT((entity->position.x + 0x3fff));
  int8_t maxy = FIXED_TO_INT((entity->position.y + 0x3fff));

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

void entity_update(entity_t *entity) {
  entity_move(entity);

  entity->tile.x = FIXED_TO_INT(entity->position.x);
  entity->tile.y = FIXED_TO_INT(entity->position.y);
}

void entity_clear_visible()
{
  entity_t *current = visible_entities;
  while (current != NULL)
  {
    entity_t *next = current->next_visible;
    current->next_visible = NULL;
    current = next;
  }
  visible_entities = NULL;
  num_visible_entities = 0;
}

void entity_add_visible(entity_t *entity)
{
  // Si la lista está vacía, añadimos la entidad
  // y salimos.
  if (visible_entities == NULL) {
    visible_entities = entity;
    entity->next_visible = NULL;
    return;
  }
}

void entity_clear_adjacent()
{
  for (uint8_t y = 0; y < MAP_HEIGHT; y++)
  {
    for (uint8_t x = 0; x < MAP_WIDTH; x++)
    {
      entity_t *current = adjacent_entities[x][y];
      while (current != NULL)
      {
        entity_t *next = current->next_adjacent;
        current->next_adjacent = NULL;
        current = next;
      }
      adjacent_entities[x][y] = NULL;
    }
  }
}

void entity_add_adjacent(entity_t *entity)
{
  entity_t *current = adjacent_entities[entity->tile.x][entity->tile.y];
  if (current == NULL) {
    adjacent_entities[entity->tile.x][entity->tile.y] = entity;
    return;
  }

  while (current->next_adjacent != NULL)
  {
    current = current->next_adjacent;
  }
  current->next_adjacent = entity;
  entity->next_adjacent = NULL;
}
