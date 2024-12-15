#include "map.h"

map_t map = {
  .header = {
    .signature = MAP_SIGNATURE,
    .name = "Default map\0",
  },
  .tiles = {
    .width = MAP_WIDTH,
    .height = MAP_HEIGHT,
    .data = {
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 3, 3, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 3, 3, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
      {1, 0, 3, 3, 0, 0, 2, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
      {1, 0, 3, 3, 0, 0, 2, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 3, 3, 0, 0, 2, 2, 9, 2, 2, 0, 1, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
      {1, 0, 3, 3, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 3, 3, 3, 0, 1, 0, 0, 0, 0, 0, 1},
      {1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 3, 0, 3, 0, 1, 0, 0, 0, 0, 0, 1},
      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 1, 0, 0, 0, 0, 0, 1},
      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 3, 3, 0, 1},
      {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 3, 0, 1},
      {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 3, 0, 1},
      {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 3, 0, 3, 0, 1},
      {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 3, 0, 3, 3, 1},
      {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
      {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    }
  },
  .textures = {
    .floor = 0,
    .ceiling = 0,
  },
  .things = {NULL}
};

/**
 * Guarda los datos de un mapa en un archivo de prueba..
 */
bool map_save_current(const char* filename)
{
  return map_save(filename, &map);
}

/**
 * Carga el mapa en la memoria del mapa actual.
 */
bool map_load_current(const char* filename)
{
  return map_load(filename, &map);
}

/**
 * Guarda un mapa en un archivo.
 */
bool map_save(const char* filename, map_t *map_target)
{
  FILE *fh = fopen(filename, "wb");
  if (fh == NULL)
  {
    return false;
  }
  fwrite(map_target, sizeof(map_t), 1, fh);
  fclose(fh);
  return true;
}

/**
 * Carga un mapa desde un archivo.
 */
bool map_load(const char* filename, map_t *map_target)
{
  FILE *fh = fopen(filename, "rb");
  if (fh == NULL)
  {
    return false;
  }
  fread(map_target, sizeof(map_t), 1, fh);
  fclose(fh);
  return true;
}

/**
 * Renderiza el mapa.
 */
void map_render()
{
  const uint16_t position_y = 168;
  const uint16_t position_x = 8;
  for (uint16_t y = 0; y < MAP_HEIGHT; y++)
  {
    for (uint16_t x = 0; x < MAP_WIDTH; x++)
    {
      if (map.tiles.data[x][y] > 0)
      {
        VIDEO_PUT_PIXEL((x + position_x), (y + position_y), 0x5);
      }
    }
  }

  for (uint16_t i = 0; i < num_entities; i++)
  {
    if (entities[i].kind != NONE) {
      VIDEO_PUT_PIXEL(
          FIXED_TO_INT(entities[i].position.x) + position_x,
          (FIXED_TO_INT(entities[i].position.y) +
           position_y),
          0x0F);
    }
  }

  VIDEO_PUT_PIXEL(
      FIXED_TO_INT(player.position.x) + position_x,
      (FIXED_TO_INT(player.position.y) + position_y),
      0x0F);
}
