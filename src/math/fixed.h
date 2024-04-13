#pragma once

#include <math.h>
#include <stdlib.h>
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
#define FIXED_DECIMALS(f) ((float)(f & FIXED_MASK) / FIXED_UNIT)
#define FIXED_ABS(a) ((a) < 0 ? -(a) : (a))
#define FIXED_FLOOR(f) (f & ~FIXED_MASK)
#define FIXED_DIV2(a, b) (b == 0 ? 0 : (fixed_t)(((int64_t)a << FIXED_BITS) / (b)))
// #define FIXED_DIV(a, b) ((FIXED_ABS(a) >> 14 >= FIXED_ABS(b)) ? (a < 0 ? MININT : MAXINT) : FIXED_DIV2((a), (b)))
#define FIXED_DIV(a, b) (fixed_t)(((int64_t)a << FIXED_BITS) / b)
#define FIXED_MUL(a, b) (fixed_t)(((int64_t)a * b) >> FIXED_BITS)

#define FIXED_COS(a) fixed_sin_table[(a + FIXANG_90) & FIXANG_MASK]
#define FIXED_SIN(a) fixed_sin_table[a]

#define FIXANG_SIZE 0x10000
#define FIXANG_MAX 0xffff
#define FIXANG_MASK 0xffff
#define FIXANG_MIN 0x0000
#define FIXANG_90 0x3fff
#define FIXANG_180 0x7fff

#define FIXANG_TO_DEG(f) (((int32_t)f * 360) / FIXANG_SIZE)

#define MAXINT ((int32_t)0x7fffffff)
#define MININT ((int32_t)0x80000000)

#define FIXED_NEG_ONE 0xFFFF0000
#define FIXED_POS_ONE 0x00010000

typedef int32_t fixed_t;
typedef uint16_t fixang_t;

extern fixed_t fixed_sin_table[FIXANG_SIZE];

void fixed_start();
