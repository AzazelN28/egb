#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "video.h"

#define FONT_DEFAULT -1
#define FONT_BUFFER_SIZE 64
#define FONT_DEFAULT_SIZE 35
#define FONT_DEFAULT_WIDTH 5
#define FONT_DEFAULT_HEIGHT 7
#define MAX_FONT_GLYPHS 256
#define MAX_FONTS 16

typedef struct font_glyph_ {
  uint8_t width;
  uint8_t *data; // width * height
} font_glyph_t;

typedef struct font_ {
  bool used;
  uint8_t height;
  font_glyph_t glyphs[MAX_FONT_GLYPHS];
} font_t;

typedef int16_t font_id_t;

extern font_id_t current_font_id;
extern font_t fonts[MAX_FONTS];
extern uint8_t font_default[MAX_FONT_GLYPHS][FONT_DEFAULT_SIZE];
extern char font_buffer[FONT_BUFFER_SIZE];

font_id_t font_load(const char *filename);
bool font_unload(font_id_t font_id);
void font_draw(uint16_t x, uint16_t y, uint8_t color, font_id_t font, const char *format, ...);
