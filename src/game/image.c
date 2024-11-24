#include "image.h"

bool image_load(const char *filename, image_t *image)
{
  FILE *fh = fopen(filename, "rb");
  if (fh == NULL)
  {
    return false;
  }
  fread(&image->signature, sizeof(uint32_t), 1, fh);
  fread(&image->flags, sizeof(uint32_t), 1, fh);
  fread(&image->width, sizeof(uint16_t), 1, fh);
  fread(&image->height, sizeof(uint16_t), 1, fh);
  if (image->flags & IMAGE_HAS_PALETTE)
  {
    image->palette = (uint8_t*)malloc(IMAGE_PALETTE_SIZE);
    fread(image->palette, IMAGE_PALETTE_SIZE, 1, fh);
  }
  image->data = (uint8_t*)malloc(image->width * image->height);
  fread(image->data, image->width * image->height, 1, fh);
  fclose(fh);
  return true;
}
