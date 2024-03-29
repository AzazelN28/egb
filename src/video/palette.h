#pragma once

#include <pc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PALETTE_BYTES 768
#define PALETTE_SIZE  256
#define PALETTE_MASK  0x3C6
#define PALETTE_READ  0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA  0x3C9

extern uint8_t palette_current[PALETTE_BYTES];

void palette_start();
void palette_get_all(uint8_t *pal);
void palette_set_all(uint8_t *pal);
void palette_set(uint8_t color, uint8_t red, uint8_t green, uint8_t blue);
