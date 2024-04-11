#pragma once

#include <stdint.h>

#define RANGE_CLAMP(v, min, max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))
#define RANGE_LINEAR(v, from, to) ((from + (to - from) * v) / 0xFFFF)

int32_t range_clamp(int32_t value, int32_t min, int32_t max);
int32_t range_linear(uint16_t progress, int32_t from, int32_t to);
