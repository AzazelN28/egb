#include "level.h"

level_t* level_create(uint16_t width, uint16_t height)
{
  level_t* level = calloc(1, sizeof(level_t));
  assert(level != NULL);
  level->width = width;
  level->height = height;
  level->data = calloc(width * height, sizeof(level_tile_t));
  return level;
}

void level_destroy(level_t* level)
{
  assert(level != NULL);
  free(level->data);
  free(level);
}
