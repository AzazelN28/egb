#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/game.h"
#include "core/log.h"

#if 0
#include <crt0.h>
int _crt0_startup_flags = _CRT0_FLAG_NULLOK | _CRT0_FLAG_LOCK_MEMORY;
#endif

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
