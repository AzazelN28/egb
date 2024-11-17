#include "game.h"

bool game_is_running = true;

uint32_t frame_tics = 0;
uint32_t frame_start = 0;
uint32_t frame_count = 0;
uint32_t frame_rate = 0;

/**
 * Leemos las opciones de la línea de comandos.
 */
void game_get_options(game_options_t *options, int argc, char** argv) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "D") == 0) {
      options->dump_defaults = true;
    }
  }
}

/**
 * Arrancamos el juego. Aquí es donde
 * registraremos todas las interrupciones
 * necesarias, reservaremos memoria y
 * pondremos el sistema en modo gráfico.
 */
bool game_init(game_options_t *options)
{
  clrscr();
  _setcursortype(_NOCURSOR);
  log_header("%s %s", GAME_NAME, GAME_VERSION);

  sleep(1);

  log_line("Iniciando mouse...");
  if (!mouse_start())
  {
    log_answer("[FAIL]");
    return false;
  }
  log_answer("[OK]");

  log_line("Iniciando keyboard...");
  if (!keyboard_start())
  {
    log_answer("[FAIL]");
    return false;
  }
  log_answer("[OK]");

  log_line("Iniciando fixed point...");
  fixed_start();
  log_answer("[OK]");

  log_line("Iniciando SoundBlaster...");
  sb_start(); // soundblaster_start();
  log_answer("[OK]");

  log_line("Iniciando raycaster...");
  raycaster_start();
  log_answer("[OK]");
  if (options->dump_defaults) {
    log_line("Guardando mapa...");
    map_save_current("_DEFAULT.MAP");
    log_answer("[OK]");
  }
  log_line("Cargando mapa...");
  if (!map_load_current("_DEFAULT.MAP"))
  {
    log_answer("[FAIL]");
  } else {
    log_answer("[OK]");
  }

  sleep(1);

  log_line("Iniciando timer...");
  timer_start();
  log_answer("[OK]");

  log_line("Iniciando video...");
  video_start();
  log_answer("[OK]");

  log_line("Iniciando paleta...");
  palette_start();
  log_answer("[OK]");
  if (options->dump_defaults) {
    log_line("Guardando paleta...");
    if (!palette_save("_DEFAULT.PAL"))
    {
      log_answer("[FAIL]");
    } else {
      log_answer("[OK]");
    }
  }
  log_line("Cargando paleta...");
  if (!palette_load("_DEFAULT.PAL"))
  {
    log_answer("[FAIL]");
  }
  else
  {
    log_answer("[OK]");
  }

  log_line("Iniciando colormap...");
  colormap_load("_DEFAULT.CMP");
  log_answer("[OK]");

  // sb_play_file("APLAUSO.PCM");
  fade_in(100);
  return true;
}

/**
 * Procesamos la entrada del usuario.
 */
void game_input() {
  // Obtenemos el estado del ratón.
  mouse_get_status();

  // Si pulsamos ALT + X, salimos del juego. Esto es muy
  // al estilo de DIV Games Studio.
  if (KEY_IS_PRESSED(KEY_LEFT_ALT) && KEY_IS_PRESSED(KEY_X))
  {
    game_is_running = false;
  }
}

/**
 * Actualizamos la lógica del juego.
 */
void game_update() {
  entity_clear_adjacent();

  if (KEY_IS_PRESSED(KEY_W) || KEY_IS_PRESSED(KEY_UP))
  {
    view.changed_position = true;
    player.next_position.x = player.position.x + FIXED_MUL(player.direction.x, 0x1000);
    player.next_position.y = player.position.y + FIXED_MUL(player.direction.y, 0x1000);
  }
  else if (KEY_IS_PRESSED(KEY_S) || KEY_IS_PRESSED(KEY_DOWN))
  {
    view.changed_position = true;
    player.next_position.x = player.position.x - FIXED_MUL(player.direction.x, 0x1000);
    player.next_position.y = player.position.y - FIXED_MUL(player.direction.y, 0x1000);
  }

  if (KEY_IS_PRESSED(KEY_A))
  {
    view.changed_position = true;
    player.next_position.x = player.position.x + FIXED_MUL(player.direction.y, 0x1000);
    player.next_position.y = player.position.y - FIXED_MUL(player.direction.x, 0x1000);
  }
  else if (KEY_IS_PRESSED(KEY_D))
  {
    view.changed_position = true;
    player.next_position.x = player.position.x - FIXED_MUL(player.direction.y, 0x1000);
    player.next_position.y = player.position.y + FIXED_MUL(player.direction.x, 0x1000);
  }

  if (KEY_IS_PRESSED(KEY_LEFT))
  {
    player.rotation -= PLAYER_ROTATION;
    view.changed_rotation = true;
  }
  else if (KEY_IS_PRESSED(KEY_RIGHT))
  {
    player.rotation += PLAYER_ROTATION;
    view.changed_rotation = true;
  }

  if (view.changed_rotation) {
    player.direction.x = FIXED_COS(player.rotation);
    player.direction.y = FIXED_SIN(player.rotation);

    view.direction.x = player.direction.x;
    view.direction.y = player.direction.y;

    view.plane.x = FIXED_MUL(-player.direction.y, 0xb000);
    view.plane.y = FIXED_MUL(player.direction.x, 0xb000);
  }

  if (view.changed_position) {
    entity_update(&player);
    entity_add_adjacent(&player);

    view.position.x = player.position.x;
    view.position.y = player.position.y;
  }

  entity_update(&other);
  entity_add_adjacent(&other);
}

void debug_render() {
  // Imprimimos el número de tics y las coordenadas del ratón.
  font_draw(0, 0, 0xF, "FPS: %d TIME: %d DURATION: %d START: %d VISIBLE: %d", frame_rate, frame_tics, fade_duration_in_tics, fade_timer.start, num_visible_entities);
#if 0
  if (mouse.current.y > columns[mouse.current.x].draw_start
   && mouse.current.y < columns[mouse.current.x].draw_end)
    font_draw(0, 10, 0xF, "x: %d y: %d s: %d", columns[mouse.current.x].x, columns[mouse.current.x].y, columns[mouse.current.x].side);
  font_draw(0, 10, 0xF, "p.x: %.02f p.y: %.02f p.rot: %03d", FIXED_TO_FLOAT(player.position.x), FIXED_TO_FLOAT(player.position.y), FIXANG_TO_DEG(player.rotation));
  font_draw(0, 30, 0xF, "v.d.x: %.02f v.d.y: %.02f", FIXED_TO_FLOAT(view.direction.x), FIXED_TO_FLOAT(view.direction.y));
  font_draw(0, 20, 0xF, "v.p.x: %.02f v.p.y: %.02f", FIXED_TO_FLOAT(view.plane.x), FIXED_TO_FLOAT(view.plane.y));
  font_draw(0, 40, 0xF, "z: %d h: %d ds: %d de: %d", FIXED_TO_INT(columns[0].z), columns[0].height, columns[0].draw_start, columns[0].draw_end);
  font_draw(0, 50, 0xF, "r.x: %.02f r.d.x: %.02f r.d.y: %.02f", FIXED_TO_FLOAT(ray.x), FIXED_TO_FLOAT(ray.direction.x), FIXED_TO_FLOAT(ray.direction.y));
  font_draw(0, 60, 0xF, "r.dd.x: %.02f r.dd.y: %.02f", FIXED_TO_FLOAT(ray.delta_dist.x), FIXED_TO_FLOAT(ray.delta_dist.y));
  font_draw(0, 70, 0xF, "r.sd.x: %.02f r.sd.y: %.02f", FIXED_TO_FLOAT(ray.side_dist.x), FIXED_TO_FLOAT(ray.side_dist.y));
  font_draw(0, 80, 0xF, "r.td.x: %.02f r.td.y: %.02f", FIXED_TO_FLOAT(ray.tile_delta.x), FIXED_TO_FLOAT(ray.tile_delta.y));
  font_draw(0, 90, 0xF, "r.s.x: %.02f r.s.y: %.02f r.s: %d r.h: %d", FIXED_TO_FLOAT(ray.step.x), FIXED_TO_FLOAT(ray.step.y), ray.side, ray.hit);
  font_draw(0, 100, 0xF, "r.t.x: %d r.t.y: %d r.i: %d", ray.tilei.x, ray.tilei.y, ray.iterations);

  VIDEO_PUT_PIXEL(mouse.current.x, mouse.current.y, 0x0F);
#endif

#if 0
  for (int8_t i = 0; i < 16; i++)
  {
    for (int8_t j = 0; j < 16; j++)
    {
      uint16_t x = i * 2;
      uint16_t y = 120 + (j * 2);
      uint8_t c = i * j;
      VIDEO_PUT_PIXEL(x, y, c);
      VIDEO_PUT_PIXEL((x + 1), y, c);
      VIDEO_PUT_PIXEL((x + 1), (y + 1), c);
      VIDEO_PUT_PIXEL(x , (y + 1), c);
    }
  }
#endif

  if (fade_in_progress)
    video_fill_rect(0, 2, 2, 2, 4);

  if (player.collision == COLLISION_X)
    video_fill_rect(0, 0, 2, 2, 1);
  else if (player.collision == COLLISION_Y)
    video_fill_rect(0, 0, 2, 2, 2);
  else if (player.collision == COLLISION_XY)
    video_fill_rect(0, 0, 2, 2, 3);
}

/**
 * Renderizamos el juego.
 */
void game_render() {
  video_render();
  fade_update();
  entity_clear_visible();
  raycaster_render();
  map_render();
  debug_render();
}

/**
 * El bucle principal del juego.
 */
void game_loop() {
  frame_tics = timer_tics;
  frame_count++;
  if (frame_tics - frame_start >= 100)
  {
    frame_rate = frame_count;
    frame_count = 0;
    frame_start = frame_tics;
  }

  game_input();
  game_update();
  game_render();
}

/**
 * Terminamos el juego. Para ello necesitamos
 * restaurar todas las interrupciones que hayamos
 * reemplazado, liberar la memoria reservada y
 * retornar al modo de texto.
 */
void game_quit() {
  // mouse_hide();
  sb_stop();
  mouse_stop();
  keyboard_stop();
  timer_stop();
  video_stop();

  log_header("EGB %s", GAME_VERSION);
  log_line("Gracias por jugar!\r\n");

  _setcursortype(_NORMALCURSOR);
}
