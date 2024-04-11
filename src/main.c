#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/game.h"
#include "core/log.h"

/*********************************************************************************************
 * MAIN
 *********************************************************************************************/
int main(int argc, char **argv) {
  if (!game_init())
  {
    log_line("Algo petó fuerte en game_init()...");
    return 1;
  }

  while (game_is_running == true) {
    game_loop();
  }
  game_quit();
  return 0;
}
