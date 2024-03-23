#pragma once

#include <stdint.h>

uint8_t rand_table[256] = {
  160, 161, 191, 128, 77, 56, 129, 26, 164, 168, 22, 188, 3, 17, 19, 63,
  44, 238, 220, 28, 34, 66, 38, 0, 155, 208, 13, 170, 240, 244, 2, 119,
  136, 25, 223, 1, 198, 169, 110, 91, 79, 166, 125, 192, 140, 147, 94, 96,
  202, 174, 36, 46, 60, 27, 205, 109, 222, 30, 23, 74, 233, 126, 141, 131,
  47, 212, 35, 41, 71, 120, 180, 252, 193, 176, 10, 200, 215, 20, 165, 189,
  15, 54, 85, 182, 68, 11, 103, 112, 123, 130, 69, 194, 178, 76, 4, 124,
  145, 40, 185, 57, 52, 42, 93, 150, 101, 16, 242, 87, 172, 72, 210, 217,
  37, 88, 199, 179, 5, 31, 159, 143, 228, 219, 253, 216, 148, 214, 133, 84,
  132, 7, 51, 229, 65, 114, 206, 62, 186, 151, 92, 138, 80, 236, 59, 64,
  105, 135, 144, 90, 39, 70, 248, 171, 146, 99, 14, 55, 201, 225, 107, 153,
  49, 33, 158, 218, 78, 243, 18, 50, 116, 227, 156, 173, 9, 117, 235, 251,
  102, 12, 58, 177, 8, 113, 207, 162, 203, 139, 250, 97, 21, 213, 104, 211,
  29, 83, 195, 95, 190, 134, 86, 184, 137, 149, 73, 61, 234, 255, 246, 181,
  142, 209, 196, 106, 163, 118, 254, 197, 67, 175, 221, 32, 122, 89, 82, 167,
  232, 239, 6, 100, 98, 157, 152, 249, 231, 183, 245, 226, 154, 43, 121, 111,
  127, 24, 81, 45, 48, 75, 108, 53, 115, 247, 230, 241, 237, 224, 187, 204,
};
uint8_t rand_index = 0;

void rand_seed(uint8_t index);
uint8_t rand_uint8();
float rand_float();
