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
  game_options_t options = {
    .dump_defaults = false,
  };

  game_get_options(&options, argc, argv);

  // Cargamos recursos, etc.
  if (!game_init(&options))
  {
    log_line("Algo petó fuerte en game_init()...");
    return 1;
  }

  // Bucle principal, aquí es donde
  // se realiza toda la magia.
  while (game_is_running == true)
  {
    game_loop();
  }
  // Liberamos memoria, guardamos posibles
  // archivos, etc.
  game_quit();
  return 0;
}
