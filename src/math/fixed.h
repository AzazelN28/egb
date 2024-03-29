#pragma once

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define FIXED_BITS 16
#define FIXED_UNIT (1 << FIXED_BITS)
#define FIXED_MASK (FIXED_UNIT - 1)

#define FIXED_FROM(i, d) ((i << FIXED_BITS) | (d & FIXED_MASK))
#define FIXED_FROM_INT(i) (i << FIXED_BITS)
#define FIXED_TO_INT(f) (f >> FIXED_BITS)
#define FIXED_FROM_FLOAT(f) ((fixed_t)(f * FIXED_UNIT))
#define FIXED_TO_FLOAT(f) ((float)(f) / FIXED_UNIT)

#define FIXANG_SIZE 0x10000
#define FIXANG_MAX 0xffff
#define FIXANG_MASK 0xffff
#define FIXANG_MIN 0x0000
#define FIXANG_90 0x3fff
#define FIXANG_180 0x7fff

typedef int32_t fixed_t;
typedef uint16_t fixang_t;

extern fixed_t fixed_sin_table[FIXANG_SIZE];

void fixed_start();
fixed_t fixed_from_int(int16_t i);
fixed_t fixed_from_float(float f);
int16_t fixed_to_int(fixed_t f);
float fixed_to_float(fixed_t f);
fixed_t fixed_mul(fixed_t a, fixed_t b);
fixed_t fixed_cos(fixang_t angle);
fixed_t fixed_sin(fixang_t angle);
