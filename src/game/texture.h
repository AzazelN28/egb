#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_TEXTURES 1024
#define TEXTURE_MASK (MAX_TEXTURES - 1)

#define TEXTURE_SIZE 64
#define TEXTURE_HALF_SIZE 32
#define TEXTURE_BUFFER_SIZE 4096
#define TEXTURE_FIXED_SIZE 0x400000
#define TEXTURE_FIXED_HALF_SIZE 0x200000

#define TRANSPARENT_COLOR 0x03

#define TEXTURE_NONE 0x10000

extern uint32_t textures_loaded;
extern uint8_t textures[MAX_TEXTURES][TEXTURE_BUFFER_SIZE];
extern uint8_t checker_texture[TEXTURE_BUFFER_SIZE];

void texture_start();
bool texture_save(const char *filename);
bool texture_load(const char *filename);
bool textures_load(const char *filename);
