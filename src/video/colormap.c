#include "colormap.h"

uint8_t colormap[COLORMAP_SHADES][COLORMAP_SIZE];

/**
 * Carga un colormap.
 */
bool colormap_load(const char *filename)
{
  FILE* fh = fopen(filename, "rb");
  if (!fh) {
    return false;
  }
  fread(colormap, COLORMAP_SHADES * COLORMAP_SIZE, 1, fh);
  fclose(fh);
  return true;
}

/**
 * Guarda un colormap.
 */
bool colormap_save(const char *filename)
{
  FILE *fh = fopen(filename, "wb");
  if (!fh) {
    return false;
  }

  fwrite(colormap, COLORMAP_SHADES * COLORMAP_SIZE, 1, fh);
  fclose(fh);
  return true;
}
