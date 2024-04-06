#include "game.h"

#define RAYCASTER_MIN_Y 20
#define RAYCASTER_MAX_Y 160

#define MAP_WIDTH 24
#define MAP_HEIGHT 24

uint32_t map[MAP_WIDTH][MAP_HEIGHT] =
  {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 1, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
  };

typedef struct vec2fix_ {
  fixed_t x, y;
} vec2fix_t;

typedef struct vec2int_ {
  int16_t x, y;
} vec2int_t;

struct view_ {
  vec2fix_t position;
  vec2fix_t direction;
  vec2fix_t plane;
} view;

struct column_
{
  fixed_t z;
  int16_t height;
  int16_t draw_start;
  int16_t draw_end;
} columns[VIDEO_WIDTH];

struct ray_
{
  bool hit;
  int16_t iterations;
  uint8_t side;
  fixed_t x;
  fixed_t perp_wall_dist;
  vec2fix_t tile;
  vec2int_t tilei;
  vec2fix_t step;
  vec2fix_t side_dist;
  vec2fix_t tile_delta;
  vec2fix_t direction;
  vec2fix_t delta_dist;
} ray = {0};

struct player_ {
  vec2fix_t position;
  vec2fix_t direction;
  fixang_t rotation;
} player = {
    {FIXED_FROM_INT(22), FIXED_FROM_INT(11)},
    {FIXED_FROM_INT(-1), FIXED_FROM_INT(0)},
    0
};

bool game_is_running = true;

uint32_t frame_start = 0;
uint32_t frame_count = 0;
uint32_t frame_rate = 0;

/**
 * Arrancamos el juego. Aquí es donde
 * registraremos todas las interrupciones
 * necesarias, reservaremos memoria y
 * pondremos el sistema en modo gráfico.
 */
void game_init()
{
  fixed_start();
  timer_start();
  keyboard_start();
  mouse_start();
  video_start();
  palette_start();
  sb_start(); // soundblaster_start();
  sb_play_file("APLAUSO.PCM");

  fade_in(100);
}

/**
 * Procesamos la entrada del usuario.
 */
void game_input() {
  // Obtenemos el estado del ratón.
  mouse_get_status();

  // Si pulsamos ALT + X, salimos del juego. Esto es muy
  // al estilo de DIV Games Studio.
  if (keys[KEY_LEFT_ALT] && keys[KEY_X])
  {
    game_is_running = false;
  }
}

/**
 * Actualizamos la lógica del juego.
 */
void game_update() {
  if (key_is_pressed(KEY_UP))
  {
    player.position.x += FIXED_MUL(player.direction.x, 0x4000);
    player.position.y += FIXED_MUL(player.direction.y, 0x4000);
  }
  else if (key_is_pressed(KEY_DOWN))
  {
    player.position.x -= FIXED_MUL(player.direction.x, 0x4000);
    player.position.y -= FIXED_MUL(player.direction.y, 0x4000);
  }

  if (key_is_pressed(KEY_LEFT)) {
    player.rotation -= 0x100;
  } else if (key_is_pressed(KEY_RIGHT)) {
    player.rotation += 0x100;
  }

  player.direction.x = FIXED_COS(player.rotation);
  player.direction.y = FIXED_SIN(player.rotation);

  view.position.x = player.position.x;
  view.position.y = player.position.y;

  view.direction.x = player.direction.x;
  view.direction.y = player.direction.y;

  view.plane.x = FIXED_MUL(-player.direction.y, 0xb000);
  view.plane.y = FIXED_MUL(player.direction.x, 0xb000);
}

void raycaster_render()
{
  for (uint16_t x = 0; x < VIDEO_WIDTH; x++)
  {
    ray.x = (FIXED_FROM_INT(2 * x) / VIDEO_WIDTH) - FIXED_UNIT;

    ray.direction.x = view.direction.x + FIXED_MUL(view.plane.x, ray.x);
    ray.direction.y = view.direction.y + FIXED_MUL(view.plane.y, ray.x);

    ray.delta_dist.x = ray.direction.x == 0 ? MAXINT : abs(fixed_div(FIXED_UNIT, ray.direction.x));
    ray.delta_dist.y = ray.direction.y == 0 ? MAXINT : abs(fixed_div(FIXED_UNIT, ray.direction.y));

    ray.tile.x = FIXED_FLOOR(view.position.x);
    ray.tile.y = FIXED_FLOOR(view.position.y);

    if (ray.direction.x < 0)
    {
      ray.step.x = FIXED_FROM_INT(-1);
      ray.tile_delta.x = (view.position.x - FIXED_FLOOR(view.position.x));
    }
    else
    {
      ray.step.x = FIXED_FROM_INT(1);
      ray.tile_delta.x = (FIXED_FLOOR(view.position.x) + FIXED_UNIT - view.position.x);
    }

    if (ray.direction.y < 0)
    {
      ray.step.y = FIXED_FROM_INT(-1);
      ray.tile_delta.y = (view.position.y - FIXED_FLOOR(view.position.y));
    }
    else
    {
      ray.step.y = FIXED_FROM_INT(1);
      ray.tile_delta.y = (FIXED_FLOOR(view.position.y) + FIXED_UNIT - view.position.y);
    }

    ray.side_dist.x = FIXED_MUL(ray.tile_delta.x, ray.delta_dist.x);
    ray.side_dist.y = FIXED_MUL(ray.tile_delta.y, ray.delta_dist.y);

    ray.hit = false;
    ray.iterations = 0;
    while (ray.hit == false)
    {
      ray.iterations++;
      if (ray.side_dist.x < ray.side_dist.y)
      {
        ray.side_dist.x += ray.delta_dist.x;
        ray.tile.x += ray.step.x;
        ray.side = 0;
      }
      else
      {
        ray.side_dist.y += ray.delta_dist.y;
        ray.tile.y += ray.step.y;
        ray.side = 1;
      }

      ray.tilei.x = FIXED_TO_INT(ray.tile.x);
      ray.tilei.y = FIXED_TO_INT(ray.tile.y);

      if (ray.tilei.x < 0 || ray.tilei.y < 0 || ray.tilei.x >= MAP_WIDTH || ray.tilei.y >= MAP_HEIGHT)
      {
        ray.side = 2; // OUTSIDE.
        ray.hit = true;
      } else if (map[ray.tilei.x][ray.tilei.y] > 0) {
        ray.hit = true;
      }
    }

    if (ray.side == 0)
      ray.perp_wall_dist = (ray.side_dist.x - ray.delta_dist.x);
    else
      ray.perp_wall_dist = (ray.side_dist.y - ray.delta_dist.y);

    columns[x].z = ray.perp_wall_dist;
    if (columns[x].z == 0)
      columns[x].z = FIXED_UNIT;

    columns[x].height = FIXED_TO_INT(fixed_div(FIXED_FROM_INT(VIDEO_HEIGHT), columns[x].z));

    // calculate lowest and highest pixel to fill in current stripe
    columns[x].draw_start = -(columns[x].height >> 1) + (VIDEO_HEIGHT >> 1);
    if (columns[x].draw_start < RAYCASTER_MIN_Y)
      columns[x].draw_start = RAYCASTER_MIN_Y;

    columns[x].draw_end = (columns[x].height >> 1) + (VIDEO_HEIGHT >> 1);
    if (columns[x].draw_end > RAYCASTER_MAX_Y)
      columns[x].draw_end = RAYCASTER_MAX_Y;

    for (int16_t y = columns[x].draw_start; y < columns[x].draw_end; y++)
    {
      uint8_t color = 0;
      if (ray.side == 1)
        color = 0x5;
      else
        color = 0x3;

      VIDEO_PUT_PIXEL(x, y, color);
    }
  }
}

void map_render() {
  const uint16_t position_y = 160;
  for (uint16_t y = 0; y < MAP_HEIGHT; y++)
  {
    for (uint16_t x = 0; x < MAP_WIDTH; x++)
    {
      if (map[x][y] > 0)
      {
        video_put_pixel(x, y + position_y, 0x5);
      }
    }
  }

  video_put_pixel(
    FIXED_TO_INT(player.position.x),
    FIXED_TO_INT(player.position.y) + position_y,
    0x0F
  );
}

void debug_render() {
  // Imprimimos el número de tics y las coordenadas del ratón.
  font_draw(0, 0, 0xF, "FPS: %d TIME: %d DURATION: %d START: %d", frame_rate, timer_tics, fade_duration_in_tics, fade_timer.start);
#if 0
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
#endif

  if (fade_in_progress)
    video_fill_rect(0, 2, 2, 2, 4);

  if (sb_playing)
    video_fill_rect(0, 0, 2, 2, 1);
}

/**
 * Renderizamos el juego.
 */
void game_render() {
  video_update();
  video_clear();
  fade_update();
  raycaster_render();
  map_render();
  debug_render();
}

/**
 * El bucle principal del juego.
 */
void game_loop() {

  frame_count++;
  if (timer_tics - frame_start >= 100)
  {
    frame_rate = frame_count;
    frame_count = 0;
    frame_start = timer_tics;
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
  video_stop();
  timer_stop();
}
