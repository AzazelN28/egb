#include <stdio.h>
#include <stdint.h>

int main(int argc, char** argv) {
  for (uint8_t j = 0; j < 16; j++) {
    for (uint8_t i = 0; i < 16; i++) {
      uint8_t character = j * 16 + i;
      if (character < 32) {
        printf("' ',");
        continue;
      }
      if (character == '\'')
        printf("'\''");
      else
        printf("'%c',", character);
    }
    printf("\n");
  }
}
