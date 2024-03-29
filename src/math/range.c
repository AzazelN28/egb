#include "range.h"

int32_t range_clamp(int32_t value, int32_t min, int32_t max)
{
  if (value < min) return min;
  if (value > max) return max;
  return value;
}
