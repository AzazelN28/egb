#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "video.h"

#define FONT_BUFFER_SIZE 64
#define FONT_DEFAULT_SIZE 5 * 7
#define FONT_DEFAULT_WIDTH 5
#define FONT_DEFAULT_HEIGHT 7
#define MAX_FONT_GLYPHS 256
#define MAX_FONTS 16

typedef struct font_glyph_ {
  uint8_t width;
  uint8_t *data; // width * height
} font_glyph_t;

typedef struct font_ {
  uint8_t height;
  font_glyph_t glyphs[256];
} font_t;

extern int16_t font_id;
extern font_t fonts[MAX_FONTS];
extern uint8_t font[MAX_FONT_GLYPHS][FONT_DEFAULT_SIZE];
extern char font_buffer[FONT_BUFFER_SIZE];

int16_t font_load(const char* filename);
void font_unload(int16_t id);
void font_draw(uint16_t x, uint16_t y, uint8_t color, const char* format, ...);
