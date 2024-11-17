#include "palette.h"

uint8_t palette_current[PALETTE_BYTES];

/**
 * Obtiene todos los colores de la paleta
 * actual.
 */
void palette_start()
{
  palette_get_all(palette_current);
}

/**
 * Obtiene todos los colores de la paleta
 * y las almacena en la memoria apuntada
 * por `pal`.
 */
void palette_get_all(uint8_t *pal)
{
  outportb(PALETTE_MASK, 0xFF);
  outportb(PALETTE_READ, 0);
  for (int i = 0; i < PALETTE_BYTES; ++i) {
    pal[i] = inportb(PALETTE_DATA);
  }
}

/**
 * Establece todos los colores de la paleta
 * con los valores almacenados en la memoria.
 */
void palette_set_all(uint8_t *pal)
{
  outportb(PALETTE_MASK, 0xFF);
  outportb(PALETTE_WRITE, 0);
  for (int i = 0; i < PALETTE_BYTES; ++i) {
    outportb(PALETTE_DATA, pal[i]);
  }
}

/**
 * Establece el color de la paleta.
 */
void palette_set(uint8_t color, uint8_t red, uint8_t green, uint8_t blue)
{
  outportb(PALETTE_MASK, 0xFF);
  outportb(PALETTE_WRITE, color);
  outportb(PALETTE_DATA, red);
  outportb(PALETTE_DATA, green);
  outportb(PALETTE_DATA, blue);
}

/**
 * Guarda la paleta en un archivo.
 */
bool palette_save(const char* filename)
{
  FILE* fh = fopen(filename, "wb");
  if (!fh) {
    return false;
  }
  size_t bytes = fwrite(palette_current, PALETTE_BYTES, 1, fh);
  bool result = bytes == PALETTE_BYTES;
  fclose(fh);
  return result;
}

/**
 * Carga la paleta de un archivo.
 */
bool palette_load(const char* filename)
{
  FILE* fh = fopen(filename, "rb");
  if (!fh) {
    return false;
  }
  size_t bytes = fread(palette_current, PALETTE_BYTES, 1, fh);
  bool result = bytes == PALETTE_BYTES;
  fclose(fh);
  return result;
}
