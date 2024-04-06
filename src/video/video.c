#include "video.h"

uint8_t *video_ptr = NULL;
uint8_t video_buffer[VIDEO_BUFFER_SIZE];

bool video_set_mode(uint32_t mode) {
  __dpmi_regs r;
  r.x.ax = mode;
  if (__dpmi_int(VIDEO_INTERRUPT, &r) == -1)
    return false;

  return true;
}

bool video_start() {
  // Arrancamos el modo 13 (320x200 256 colores)
  video_set_mode(VIDEO_MODE_320x200);

  // Activamos el acceso a punteros en la memoria
  // cercana.
  if (__djgpp_nearptr_enable() == 0)
    return false;

  video_ptr = (uint8_t *)(__djgpp_conventional_base + VIDEO_VGA_ADDRESS);

#ifdef VIDEO_ENABLE_UNCHAINED
  video_unchained_enable();
#endif

  return true;
}

void video_stop() {
  // Desactivamos el acceso a punteros en la memoria
  // cercana.
  __djgpp_nearptr_disable();

  // Restauramos el modo de texto.
  video_set_mode(VIDEO_MODE_TEXT);
}

void video_copy() {
  memcpy(video_ptr, video_buffer, VIDEO_BUFFER_SIZE);
}

void video_sync() {
  while (inportb(VIDEO_INPUT_STATUS) & VIDEO_VERTICAL_RETRACE);
  while (!(inportb(VIDEO_INPUT_STATUS) & VIDEO_VERTICAL_RETRACE));
}

void video_update() {
  while (inportb(VIDEO_INPUT_STATUS) & VIDEO_VERTICAL_RETRACE);
  while (!(inportb(VIDEO_INPUT_STATUS) & VIDEO_VERTICAL_RETRACE));
  memcpy(video_ptr, video_buffer, VIDEO_BUFFER_SIZE);
}

void video_clear() {
  memset(video_buffer, 0, VIDEO_BUFFER_SIZE);
}

void video_unchained_enable()
{
  // Desactivamos el modo encadenado.
  outportb(VGA_SC_INDEX, VGA_MEMORY_MODE);
  outportb(VGA_SC_DATA, 0x06);

  outportw(VGA_SC_INDEX, VGA_ALL_PLANES);
  for (int32_t i = 0; i < 0x4000; i++)
  {
    video_ptr[i] = 0x00;
  }

  // Desactivamos la escritura de longs (4 bytes).
  outportb(VGA_CRTC_INDEX, VGA_UNDERLINE_LOC);
  outportb(VGA_CRTC_DATA, 0x00);

  // Activamos la escritura de bytes (1 byte).
  outportb(VGA_CRTC_INDEX, VGA_MODE_CONTROL);
  outportb(VGA_CRTC_DATA, 0xe3);
}

void video_unchained_clear() { memset(video_buffer, 0, VIDEO_BUFFER_SIZE); }

void video_unchained_set_plane_mask(uint8_t plane)
{
  outportb(VGA_SC_INDEX, VGA_MAP_MASK);
  outportb(VGA_SC_DATA, 1 << (plane & VGA_PLANE_MASK));
}

void video_put_pixel(uint16_t x, uint16_t y, uint8_t color) {
  video_buffer[y * VIDEO_WIDTH + x] = color;
}

void video_put_pixel_safe(uint16_t x, uint16_t y, uint8_t color) {
  if (x >= 0 && x < VIDEO_WIDTH && y >= 0 && y < VIDEO_HEIGHT)
    VIDEO_PUT_PIXEL(x, y, color);
}

uint8_t video_get_pixel(uint16_t x, uint16_t y) {
  return video_buffer[y * VIDEO_WIDTH + x];
}

void video_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                            uint8_t color) {
  for (uint16_t j = y; j < y + h; j++) {
    uint32_t offset = j * VIDEO_WIDTH;
    for (uint16_t i = x; i < x + w; i++) {
      video_buffer[offset + i] = color;
    }
  }
}

void video_stroke_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                              uint8_t color) {
  for (int32_t i = x; i < x + w; i++) {
    VIDEO_PUT_PIXEL(i, y, color);
    VIDEO_PUT_PIXEL(i, y + h, color);
  }

  for (int32_t i = y; i < y + h; i++) {
    VIDEO_PUT_PIXEL(x, i, color);
    VIDEO_PUT_PIXEL(x + w, i, color);
  }
}

// @see https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void video_draw_line(uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by, uint8_t color)
{
  uint16_t dx = abs(ax - bx);
  uint16_t sx = ax < bx ? 1 : -1;
  uint16_t dy = -abs(ay - by);
  uint16_t sy = ay < by ? 1 : -1;
  uint16_t err = dx + dy;

  while (true) {
    VIDEO_PUT_PIXEL(ax, ay, color);
    if (ax == bx && ay == by)
      break;

    uint16_t e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      ax += sx;
    }
    if (e2 <= dx) {
      err += dx;
      ay += sy;
    }
  }
}
