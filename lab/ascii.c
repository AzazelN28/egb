#include <stdio.h>
#include <stdint.h>

int main(int argc, char** argv) {
  printf("\t0 1 2 3 4 5 6 7 8 9 A B C D E F \n");
  for (uint8_t j = 0; j < 16; j++) {
    printf("%X\t", j);
    for (uint8_t i = 0; i < 16; i++) {
      uint8_t character = j * 16 + i;
      if (character < 32) {
        printf("  ");
        continue;
      }
      printf("%c ", character);
    }
    printf("\n");
  }
}
