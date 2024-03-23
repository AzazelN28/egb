#include "rand.h"

void rand_seed(uint8_t index) {
  rand_index = index;
}

uint8_t rand_uint8()
{
  return rand_table[rand_index++];
}

float rand_float() {
  return (float)rand_uint8() / 255.0f;
}
