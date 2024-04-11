#include "range.h"

int32_t range_clamp(int32_t value, int32_t min, int32_t max)
{
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

int32_t range_linear(uint16_t progress, int32_t from, int32_t to)
{
  return (from + (to - from) * progress) / 0xFFFF;
}
