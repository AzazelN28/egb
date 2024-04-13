#include "fixed.h"

fixed_t fixed_sin_table[FIXANG_SIZE];

void fixed_start()
{
  FILE* file = fopen("sin.bin", "rb");
  if (file == NULL) {
    for (fixang_t i = 0; i < FIXANG_MAX; i++)
    {
      float angle = (float)i / (float)FIXANG_MAX * M_PI * 2.0;
      fixed_sin_table[i] = FIXED_FROM_FLOAT(sinf(angle));
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
