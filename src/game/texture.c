#include "texture.h"

uint32_t textures_loaded = 0;
uint8_t textures[MAX_TEXTURES][TEXTURE_BUFFER_SIZE];
uint8_t checker_texture[TEXTURE_BUFFER_SIZE] = {0};

/**
 * Genera la textura de debug.
 */
void texture_start()
{
#ifdef XOR_TEXTURE
  for (int y = 0; y < TEXTURE_SIZE; y++)
  {
    for (int x = 0; x < TEXTURE_SIZE; x++)
    {
      uint8_t c = x ^ y;
      checker_texture[y * TEXTURE_SIZE + x] = c;
    }
  }
#else
  // Creamos una checkerboard texture para probar.
  uint8_t fg = 0x03, bg = 0x05, num_cols = 4, num_rows = 4, col = 0, row = 0;
  for (int y = 0; y < TEXTURE_SIZE; y++)
  {
    row = (y / (TEXTURE_SIZE / num_rows));
    for (int x = 0; x < TEXTURE_SIZE; x++)
    {
      col = (x / (TEXTURE_SIZE / num_cols));
      checker_texture[y * TEXTURE_SIZE + x] = (row + col) % 2 ? fg : bg;
    }
  }
#endif
}

/**
 * Guarda la textura de checkerboard que se usa
 * para debuggear.
 */
bool texture_save(const char* filename)
{
  FILE* fh = fopen(filename, "wb");
  if (fh == NULL) {
    return false;
  }
  fwrite(checker_texture, TEXTURE_BUFFER_SIZE, 1, fh);
  fclose(fh);
  return true;
}

/**
 * Carga la textura de checkerboard que se usa
 * para debuggear.
 */
bool texture_load(const char *filename)
{
  FILE *fh = fopen(filename, "rb");
  if (fh == NULL)
  {
    return false;
  }
  fread(checker_texture, TEXTURE_BUFFER_SIZE, 1, fh);
  fclose(fh);
  return true;
}

/**
 * Carga las texturas del juego.
 */
bool textures_load(const char *filename)
{
  FILE *fh = fopen(filename, "rb");
  if (fh == NULL)
  {
    return false;
  }
  for (int i = 0; i < MAX_TEXTURES; i++)
  {
    fread(textures[i], TEXTURE_BUFFER_SIZE, 1, fh);
    textures_loaded++;
  }
  fclose(fh);
  return true;
}
