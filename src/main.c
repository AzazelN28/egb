#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/game.h"

/*********************************************************************************************
 * MAIN
 *********************************************************************************************/
int main(int argc, char **argv) {
  game_init();
  while (game_is_running == true) {
    game_loop();
  }
  game_quit();
  return 0;
}
