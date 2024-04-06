#include "fixed.h"

fixed_t fixed_sin_table[FIXANG_SIZE];

void fixed_start()
{
  FILE* file = fopen("sin.bin", "rb");
  if (file == NULL) {
    for (fixang_t i = 0; i < FIXANG_MAX; i++)
    {
      float angle = (float)i / (float)FIXANG_MAX * M_PI * 2.0;
      fixed_sin_table[i] = fixed_from_float(sinf(angle));
    }

    // Intentamos guardar la tabla del seno
    // al disco.
    file = fopen("sin.bin", "wb");
    if (file == NULL)
      return;

    fwrite(fixed_sin_table, sizeof(fixed_t), FIXANG_SIZE, file);
    fclose(file);
  } else {
    fread(fixed_sin_table, sizeof(fixed_t), FIXANG_SIZE, file);
    fclose(file);
  }
}

fixed_t fixed_from_int(int16_t i)
{
  return i << FIXED_BITS;
}

fixed_t fixed_from_float(float f)
{
  return (fixed_t)(f * FIXED_UNIT);
}

int16_t fixed_to_int(fixed_t f)
{
  return f >> FIXED_BITS;
}

float fixed_to_float(fixed_t f)
{
  return (float)f / FIXED_UNIT;
}

fixed_t fixed_mul(fixed_t a, fixed_t b)
{
  return ((int64_t)a * (int64_t)b) >> FIXED_BITS;
}

fixed_t fixed_div2(fixed_t a, fixed_t b)
{
  int64_t c;
  c = ((int64_t)a << FIXED_BITS) / ((int64_t)b);
  return (fixed_t) c;
#if 0
  double c;
  c = ((double)a) / ((double)b) * FIXED_UNIT;
  if (c >= 2147483648.0 || c < -2147483648.0)
  {
    // I_Error("FixedDiv: divide by zero");
  }
  return (fixed_t)c;
#endif
}

fixed_t fixed_div(fixed_t a, fixed_t b)
{
  if (abs(a) >> 14 >= abs(b))
    return a < 0 ? MININT : MAXINT;
  return fixed_div2(a, b);
}

fixed_t fixed_cos(fixang_t angle)
{
  return fixed_sin_table[(angle + FIXANG_90) & FIXANG_MASK];
}

fixed_t fixed_sin(fixang_t angle)
{
  return fixed_sin_table[angle];
}
