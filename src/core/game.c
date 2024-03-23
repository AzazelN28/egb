#include "game.h"

#define NUM_POINTS 8
#define DEFAULT_Z 3

typedef struct vec3f_ {
  float x, y, z;
} vec3f_t;

vec3f_t points[NUM_POINTS] = {
  { .x = -1, .y = -1, .z = -1 },
  { .x =  1, .y = -1, .z = -1 },
  { .x =  1, .y =  1, .z = -1 },
  { .x = -1, .y =  1, .z = -1 },
  { .x = -1, .y = -1, .z =  1 },
  { .x =  1, .y = -1, .z =  1 },
  { .x =  1, .y =  1, .z =  1 },
  { .x = -1, .y =  1, .z =  1 }
};
vec3f_t transformed[NUM_POINTS] = {0};
vec3f_t projected[NUM_POINTS] = {0};

bool game_is_running = true;

/**
 * Arrancamos el juego. Aquí es donde
 * registraremos todas las interrupciones
 * necesarias, reservaremos memoria y
 * pondremos el sistema en modo gráfico.
 */
void game_init() {
  timer_start();
  keyboard_start();
  mouse_start();
  video_start();
  sb_start();
  sb_play_file("APLAUSO.PCM");
  // mouse_show();
}

void rotate(float pitch, float roll, float yaw) {
  float cosa = cos(yaw);
  float sina = sin(yaw);

  float cosb = cos(pitch);
  float sinb = sin(pitch);

  float cosc = cos(roll);
  float sinc = sin(roll);

  float Axx = cosa * cosb;
  float Axy = cosa * sinb * sinc - sina * cosc;
  float Axz = cosa * sinb * cosc + sina * sinc;

  float Ayx = sina * cosb;
  float Ayy = sina * sinb * sinc + cosa * cosc;
  float Ayz = sina * sinb * cosc - cosa * sinc;

  float Azx = -sinb;
  float Azy = cosb * sinc;
  float Azz = cosb * cosc;
  for (uint32_t index = 0; index < NUM_POINTS; index++)
  {
    transformed[index].x = Axx * points[index].x + Axy * points[index].y + Axz * points[index].z;
    transformed[index].y = Ayx * points[index].x + Ayy * points[index].y + Ayz * points[index].z;
    transformed[index].z = Azx * points[index].x + Azy * points[index].y + Azz * points[index].z;
  }

  for (uint32_t index = 0; index < NUM_POINTS; index++)
  {
    projected[index].x = (transformed[index].x / (transformed[index].z + DEFAULT_Z)) * 100 + 160;
    projected[index].y = (transformed[index].y / (transformed[index].z + DEFAULT_Z)) * 100 + 100;
  }
}

/**
 * El bucle principal del juego.
 */
void game_loop() {

  // Obtenemos el estado del ratón.
  mouse_get_status();

  // Esperamos a que se produzca el
  // retrazo vertical.
  video_sync();

  // Pasamos nuestro buffer interno
  // a la memoria de video.
  video_copy();
  video_clear();

  rotate(tics * 0.01, tics * 0.01, 0);

  video_draw_line(
    (uint32_t)projected[0].x, (uint32_t)projected[0].y,
    (uint32_t)projected[1].x, (uint32_t)projected[1].y,
    14
  );

  video_draw_line(
      (uint32_t)projected[1].x, (uint32_t)projected[1].y,
      (uint32_t)projected[2].x, (uint32_t)projected[2].y,
      12);

  video_draw_line(
      (uint32_t)projected[2].x, (uint32_t)projected[2].y,
      (uint32_t)projected[3].x, (uint32_t)projected[3].y,
      11);

  video_draw_line(
      (uint32_t)projected[3].x, (uint32_t)projected[3].y,
      (uint32_t)projected[0].x, (uint32_t)projected[0].y,
      10);

  video_draw_line(
      (uint32_t)projected[4].x, (uint32_t)projected[4].y,
      (uint32_t)projected[5].x, (uint32_t)projected[5].y,
      14);

  video_draw_line(
      (uint32_t)projected[5].x, (uint32_t)projected[5].y,
      (uint32_t)projected[6].x, (uint32_t)projected[6].y,
      12);

  video_draw_line(
      (uint32_t)projected[6].x, (uint32_t)projected[6].y,
      (uint32_t)projected[7].x, (uint32_t)projected[7].y,
      11);

  video_draw_line(
      (uint32_t)projected[7].x, (uint32_t)projected[7].y,
      (uint32_t)projected[4].x, (uint32_t)projected[4].y,
      10);

  for (uint32_t index = 0; index < 8; index++)
  {
    video_fill_rect((uint32_t)projected[index].x - 1, (uint32_t)projected[index].y - 1, 3, 3, 15);
  }

  if (sb_playing)
    video_fill_rect(0, 0, 2, 2, 1);

  // Imprimimos el número de tics y las coordenadas del ratón.
  /*
  printf(
    "Tics: %03d %03d %03d %03d %d %d %d %04d\r",
    mouse.current.x,
    mouse.current.y,
    mouse.delta.x,
    mouse.delta.y,
    mouse.buttons,
    mouse.left,
    mouse.right,
    tics
  );
  */

  // Si pulsamos ALT + X, salimos del juego. Esto es muy
  // al estilo de DIV Games Studio.
  if (keys[KEY_LEFT_ALT] && keys[KEY_X]) {
    game_is_running = false;
  }
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
