#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define IMAGE_SIGNATURE 0x494d4730
#define IMAGE_HAS_PALETTE 0x01
#define IMAGE_PALETTE_SIZE 768

typedef struct image_
{
  uint32_t signature;
  uint32_t flags;
  uint16_t width;
  uint16_t height;
  uint8_t *palette;
  uint8_t *data;
} image_t;

bool image_load(const char *filename, image_t *image);
