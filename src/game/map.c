#include "map.h"

uint32_t map[MAP_WIDTH][MAP_HEIGHT] =
  {
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
    {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 3, 0, 3, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 3, 0, 3, 3, 1},
    {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
  };

bool map_save_default()
{
  FILE *file = fopen("test.map", "wb");
  const char *map_name = "This is a test map";
  if (file == NULL)
  {
    return false;
  }
  const uint32_t map_version = MAP_VERSION;
  const uint8_t map_width = MAP_WIDTH;
  const uint8_t map_height = MAP_HEIGHT;
  fwrite(&map_version, sizeof(uint32_t), 1, file);
  fwrite(map_name, sizeof(uint8_t), MAP_MAX_NAME_SIZE, file);
  fwrite(&map_width, sizeof(uint8_t), 1, file);
  fwrite(&map_height, sizeof(uint8_t), 1, file);
  fwrite(map, sizeof(uint32_t), MAP_WIDTH * MAP_HEIGHT, file);
  fclose(file);
  return true;
}

bool map_save(const char* filename, map_t *map)
{
  FILE *file = fopen(filename, "wb");
  if (file == NULL)
  {
    return false;
  }
  fwrite(map, sizeof(map_t), 1, file);
  fclose(file);
  return true;
}

bool map_load(const char* filename, map_t *map)
{
  FILE *file = fopen(filename, "rb");
  if (file != NULL)
  {
    return false;
  }
  fread(map, sizeof(map_t), 1, file);
  fclose(file);
  return true;
}

void map_render()
{
  const uint16_t position_y = 160;
  for (uint16_t y = 0; y < MAP_HEIGHT; y++)
  {
    for (uint16_t x = 0; x < MAP_WIDTH; x++)
    {
      if (map[x][y] > 0)
      {
        VIDEO_PUT_PIXEL(x, (y + position_y), 0x5);
      }
    }
  }

  VIDEO_PUT_PIXEL(
      FIXED_TO_INT(player.position.x),
      (FIXED_TO_INT(player.position.y) + position_y),
      0x0F);
}
