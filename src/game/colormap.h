#pragma once

#include "../video/palette.h"
#include <stdint.h>

#define COLORMAP_SHADES 16
#define COLORMAP_SIZE PALETTE_SIZE
#define COLORMAP_SHADE_MASK 0x0f
#define COLORMAP_SHADE_MULTIPLIER 1 // * 2

extern uint8_t colormap[COLORMAP_SHADES][COLORMAP_SIZE];

bool colormap_load(const char* filename);
bool colormap_save(const char* filename);
