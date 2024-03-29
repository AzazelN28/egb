#include "game.h"

#define MAP_WIDTH 24
#define MAP_HEIGHT 24

int worldMap[MAP_WIDTH][MAP_HEIGHT] =
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

bool game_is_running = true;

uint32_t frame_start = 0;
uint32_t frame_count = 0;
uint32_t frame_rate = 0;

float posX = 22, posY = 12;      // x and y start position
float dirX = -1, dirY = 0;       // initial direction vector
float planeX = 0, planeY = 0.66; // the 2d raycaster version of camera plane

float moveSpeed = 0.1; // the constant value is in squares/second
float rotSpeed = 0.1;
float posRotSpeedX;
float posRotSpeedY;
float negRotSpeedX;
float negRotSpeedY;

typedef struct wall_ {
  uint16_t yStart, yEnd;
  float z;
  uint8_t color;
} wall_t;

wall_t walls[VIDEO_WIDTH];

uint32_t w = VIDEO_WIDTH;
uint32_t h = VIDEO_HEIGHT;
uint32_t hh = 100;

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
  sb_start();
  sb_play_file("APLAUSO.PCM");

  fade_in(100);

  // mouse_show();
  posRotSpeedX = cosf(rotSpeed);
  posRotSpeedY = sinf(rotSpeed);
  negRotSpeedX = cosf(-rotSpeed);
  negRotSpeedY = sinf(-rotSpeed);
}

/**
 * El bucle principal del juego.
 */
void game_loop() {

  // Obtenemos el estado del ratón.
  mouse_get_status();

  frame_count++;
  if (timer_tics - frame_start >= 100) {
    frame_rate = frame_count;
    frame_count = 0;
    frame_start = timer_tics;
  }

  video_update();
  video_clear();

  /*
  // move forward if no wall in front of you
  if (keys[KEY_UP])
  {
    if (worldMap[(int)floorf(posX + dirX * moveSpeed)][(int)floorf(posY)] == false)
      posX += dirX * moveSpeed;
    if (worldMap[(int)floorf(posX)][(int)floorf(posY + dirY * moveSpeed)] == false)
      posY += dirY * moveSpeed;
  }
  // move backwards if no wall behind you
  if (keys[KEY_DOWN])
  {
    if (worldMap[(int)floorf(posX - dirX * moveSpeed)][(int)floorf(posY)] == false)
      posX -= dirX * moveSpeed;
    if (worldMap[(int)floorf(posX)][(int)floorf(posY - dirY * moveSpeed)] == false)
      posY -= dirY * moveSpeed;
  }
  // rotate to the right
  if (keys[KEY_RIGHT])
  {
    // both camera direction and camera plane must be rotated
    float oldDirX = dirX;
    dirX = dirX * negRotSpeedX - dirY * negRotSpeedY;
    dirY = oldDirX * negRotSpeedY + dirY * negRotSpeedX;
    float oldPlaneX = planeX;
    planeX = planeX * negRotSpeedX - planeY * negRotSpeedY;
    planeY = oldPlaneX * negRotSpeedY + planeY * negRotSpeedX;
  }
  // rotate to the left
  if (keys[KEY_LEFT])
  {
    // both camera direction and camera plane must be rotated
    float oldDirX = dirX;
    dirX = dirX * posRotSpeedX - dirY * posRotSpeedY;
    dirY = oldDirX * posRotSpeedY + dirY * posRotSpeedX;
    float oldPlaneX = planeX;
    planeX = planeX * posRotSpeedX - planeY * posRotSpeedY;
    planeY = oldPlaneX * posRotSpeedY + planeY * posRotSpeedX;
  }

  for (uint16_t x = 0; x < VIDEO_WIDTH; x++) {
    float cameraX = 2.0 * (float)(x) / (float)(w) - 1.0; // x-coordinate in camera space
    float rayDirX = dirX + planeX * cameraX;
    float rayDirY = dirY + planeY * cameraX;
    // which box of the map we're in
    int mapX = (int)floorf(posX);
    int mapY = (int)floorf(posY);

    // length of ray from current position to next x or y-side
    float sideDistX;
    float sideDistY;

    // length of ray from one x or y-side to next x or y-side
    float deltaDistY = (rayDirY == 0) ? 1e30 : fabsf(1 / rayDirY);
    float deltaDistX = (rayDirX == 0) ? 1e30 : fabsf(1 / rayDirX);
    float perpWallDist;

    // what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    int hit = 0; // was there a wall hit?
    int side;    // was a NS or a EW wall hit?

    if (rayDirX < 0)
    {
      stepX = -1;
      sideDistX = (posX - mapX) * deltaDistX;
    }
    else
    {
      stepX = 1;
      sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    }
    if (rayDirY < 0)
    {
      stepY = -1;
      sideDistY = (posY - mapY) * deltaDistY;
    }
    else
    {
      stepY = 1;
      sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }

    // perform DDA
    while (hit == 0)
    {
      // jump to next map square, either in x-direction, or in y-direction
      if (sideDistX < sideDistY)
      {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      }
      else
      {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }
      // Check if ray has hit a wall
      if (worldMap[mapX][mapY] > 0)
        hit = 1;
    }

    if (side == 0)
      perpWallDist = (sideDistX - deltaDistX);
    else
      perpWallDist = (sideDistY - deltaDistY);

    // Calculate height of line to draw on screen
    int lineHeight = (int)floorf(h / perpWallDist);

    // calculate lowest and highest pixel to fill in current stripe
    int drawStart = -(lineHeight >> 1) + hh;
    if (drawStart < 0)
      drawStart = 0;

    int drawEnd = (lineHeight >> 1) + hh;
    if (drawEnd >= h)
      drawEnd = h - 1;

    walls[x].yStart = drawStart;
    walls[x].yEnd = drawEnd;
    walls[x].z = perpWallDist;

    uint8_t color = 0;
    switch (worldMap[mapX][mapY])
    {
    case 1:
      color = side == 0 ? 1 : 9;
      break; // red
    case 2:
      color = side == 0 ? 2 : 10;
      break; // green
    case 3:
      color = side == 0 ? 3 : 11;
      break; // blue
    case 4:
      color = side == 0 ? 4 : 12;
      break; // white
    default:
      color = side == 0 ? 5 : 13;
      break; // yellow
    }
    walls[x].color = color;
  }

  for (uint16_t y = 0; y < VIDEO_HEIGHT; y++)
  {
    for (uint16_t x = 0; x < VIDEO_WIDTH; x++)
    {
      uint8_t color = walls[x].color;
      uint16_t yStart = walls[x].yStart;
      uint16_t yEnd = walls[x].yEnd;
      if (y >= yStart && y <= yEnd)
        video_put_pixel(x, y, color);
    }
  }
  */

  fade_update();

  if (keys[KEY_UP]) {
    fade_out(100);
  }
  if (keys[KEY_DOWN]) {
    fade_in(100);
  }
  if (keys[KEY_LEFT]) {
    fade_color(100, 63, 0, 0);
  }
  if (keys[KEY_RIGHT]) {
    fade_color(100, 63, 63, 63);
  }

  // Imprimimos el número de tics y las coordenadas del ratón.
  font_draw(0, 0, 0xF, "FPS: %d TIME: %d DURATION: %d START: %d", frame_rate, timer_tics, fade_duration_in_tics, fade_timer.start);

  if (fade_in_progress)
    video_fill_rect(0, 2, 2, 2, 4);

  if (sb_playing)
    video_fill_rect(0, 0, 2, 2, 1);

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
