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

void inline video_copy() { memcpy(video_ptr, video_buffer, VIDEO_BUFFER_SIZE); }

void inline video_sync() {
  while (inportb(VIDEO_INPUT_STATUS) & VIDEO_VERTICAL_RETRACE)
    ;
  while (!(inportb(VIDEO_INPUT_STATUS) & VIDEO_VERTICAL_RETRACE))
    ;
}

void inline video_clear() {
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

void inline video_put_pixel(uint32_t x, uint32_t y, uint8_t color) {
  video_buffer[y * VIDEO_WIDTH + x] = color;
}

void inline video_put_pixel_safe(uint32_t x, uint32_t y, uint8_t color) {
  if (x >= 0 && x < VIDEO_WIDTH && y >= 0 && y < VIDEO_HEIGHT)
    video_put_pixel(x, y, color);
}

uint8_t inline video_get_pixel(uint32_t x, uint32_t y) {
  return video_buffer[y * VIDEO_WIDTH + x];
}

void inline video_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                            uint8_t color) {
  for (int32_t j = y; j < y + h; j++) {
    uint32_t offset = j * VIDEO_WIDTH;
    for (int32_t i = x; i < x + w; i++) {
      video_buffer[offset + i] = color;
    }
  }
}

void inline video_stroke_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                              uint8_t color) {
  for (int32_t i = x; i < x + w; i++) {
    video_put_pixel(i, y, color);
    video_put_pixel(i, y + h, color);
  }

  for (int32_t i = y; i < y + h; i++) {
    video_put_pixel(x, i, color);
    video_put_pixel(x + w, i, color);
  }
}

// @see https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void video_draw_line(uint32_t ax, uint32_t ay, uint32_t bx, uint32_t by, uint8_t color)
{
  int32_t dx = abs(ax - bx);
  int32_t sx = ax < bx ? 1 : -1;
  int32_t dy = -abs(ay - by);
  int32_t sy = ay < by ? 1 : -1;
  int32_t err = dx + dy;

  while (true) {
    video_put_pixel(ax, ay, color);
    if (ax == bx && ay == by)
      break;

    int32_t e2 = 2 * err;
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
