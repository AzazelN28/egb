#include "entity.h"

entity_t player = {
    PLAYER,
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

entity_t entities[MAX_ENTITIES] = {
  {ENEMY,
  FIXED_HALF_UNIT,
  {FIXED_FROM_INT(22), FIXED_FROM_INT(10)},
  {FIXED_FROM_INT(22), FIXED_FROM_INT(10)},
  {FIXED_NEG_ONE, 0},
  0,
  0,
  {22, 10},
  0,
  NULL,
  NULL},
  {ENEMY,
  FIXED_HALF_UNIT,
  {FIXED_FROM_INT(10), FIXED_FROM_INT(10)},
  {FIXED_FROM_INT(10), FIXED_FROM_INT(10)},
  {FIXED_NEG_ONE, 0},
  0,
  0,
  {10, 10},
  0,
  NULL,
  NULL},
  {ENEMY,
  FIXED_HALF_UNIT,
  {FIXED_FROM_INT(9), FIXED_FROM_INT(9)},
  {FIXED_FROM_INT(9), FIXED_FROM_INT(9)},
  {FIXED_NEG_ONE, 0},
  0,
  0,
  {9, 9},
  0,
  NULL,
  NULL},
  {NONE}
};
entity_t *adjacent_entities[MAP_WIDTH][MAP_HEIGHT] = {NULL};
entity_t *visible_entities = NULL;
entity_t *last_visible_entity = NULL;
uint8_t num_visible_entities = 0;
uint8_t num_entities = 3;

/**
 * Intenta mover una entidad.
 */
bool entity_try_move(entity_t *entity)
{
  // 0x3fff == 0.25
  // 0x7fff == 0.5
  // 0xffff == 0.9999
  int8_t minx = FIXED_TO_INT((entity->position.x - ENTITY_RADIUS));
  int8_t miny = FIXED_TO_INT((entity->position.y - ENTITY_RADIUS));
  int8_t maxx = FIXED_TO_INT((entity->position.x + ENTITY_RADIUS));
  int8_t maxy = FIXED_TO_INT((entity->position.y + ENTITY_RADIUS));

  if (minx < 0 || maxx >= MAP_WIDTH) return false;
  if (miny < 0 || maxy >= MAP_HEIGHT) return false;

  for (int8_t y = miny; y <= maxy; y++)
  {
    for (int8_t x = minx; x <= maxx; x++)
    {
      if (map.tiles.data[x][y] != 0)
      {
        return false;
      }
    }
  }
  return true;
}

/**
 * Se encarga del movimiento de entidades.
 */
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

/**
 * Actualiza las entidades.
 */
void entity_update(entity_t *entity) {
  entity_move(entity);

  entity->tile.x = FIXED_TO_INT(entity->position.x);
  entity->tile.y = FIXED_TO_INT(entity->position.y);
}

/**
 * Limpia la lista de entidades visibles.
 */
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
  last_visible_entity = NULL;
  num_visible_entities = 0;
}

/**
 * Añade una entidad a la lista de entidades
 * visibles.
 */
void entity_add_visible(entity_t *entity)
{
  // Si la lista está vacía, añadimos la entidad
  // y salimos.
  if (visible_entities == NULL) {
    visible_entities = entity;
    last_visible_entity = entity;
    entity->next_visible = NULL;
    return;
  }
  last_visible_entity->next_visible = entity;
  entity->next_visible = NULL;
}

/**
 * Limpia la lista de entidades adyacentes.
 */
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

/**
 * Añade una entidad a la lista de entidades
 * adyacentes.
 */
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
