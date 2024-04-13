#include "video.h"

uint8_t *video_ptr = NULL;
uint8_t video_buffer[VIDEO_BUFFER_SIZE];
#ifdef VIDEO_ENABLE_UNCHAINED
uint32_t video_pages[VIDEO_PAGES] = {
  0x0000,
  0x4000,
  0x8000,
  0xC000
};
#endif

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

void video_render()
{
#ifdef VIDEO_ENABLE_VSYNC
  // v-sync
  while (inportb(VIDEO_INPUT_STATUS) & VIDEO_VERTICAL_RETRACE);
  while (!(inportb(VIDEO_INPUT_STATUS) & VIDEO_VERTICAL_RETRACE));
#endif
#ifdef VIDEO_ENABLE_UNCHAINED
  outportb(VGA_SC_INDEX, VGA_MAP_MASK);
  for (uint8_t plane = 0; plane < 4; plane++)
  {
    outportb(VGA_SC_DATA, 1 << (plane & VGA_PLANE_MASK));
    uint32_t offset = 0;
    for (uint16_t y = 0; y < VIDEO_HEIGHT; y++)
    {
      for (uint16_t x = plane; x < VIDEO_WIDTH; x += 4)
      {
        uint8_t c = video_buffer[y * VIDEO_WIDTH + x];
        video_ptr[offset + (x >> 2)] = c;
      }
      offset += VIDEO_PLANE_WIDTH;
    }
  }
/*
  outportb(VGA_SC_INDEX, VGA_MAP_MASK);
  for (uint8_t plane = 0; plane < 4; plane++) {
    outportb(VGA_SC_DATA, 1 << (plane & VGA_PLANE_MASK));
    for (uint8_t y = 0; y < 200; y++)
    {
      for (uint8_t px = 0; px < 80; px++)
      {
        uint16_t x = (px << 2);
        video_ptr[y * 80 + px] = video_buffer[y * 320 + x];
      }
    }
  }
*/
#else
  memcpy(video_ptr, video_buffer, VIDEO_BUFFER_SIZE);
#endif
#ifdef VIDEO_ENABLE_CLEAR
  // ESTO SE ZAMPA UNA CANTIDAD DE TIEMPO ABSURDA.
  memset(video_buffer, 0, VIDEO_BUFFER_SIZE);
#else
  memset(video_buffer, 0, 40 * 320);
#endif
}

#ifdef VIDEO_ENABLE_UNCHAINED
void video_clear()
{
  outportb(VGA_SC_INDEX, VGA_MAP_MASK); /* select plane */
  outportw(VGA_SC_DATA, VGA_ALL_PLANES);
  memset(video_ptr, 0, 0x4000);
}
#else
void video_clear() {
  memset(video_buffer, 0, VIDEO_BUFFER_SIZE);
}
#endif

void video_unchained_enable()
{
  // Desactivamos el modo encadenado.
  outportb(VGA_SC_INDEX, VGA_MEMORY_MODE);
  outportb(VGA_SC_DATA, 0x06);

  outportw(VGA_SC_INDEX, VGA_ALL_PLANES);
  memset(video_ptr, 0, 0x4000);

  // Desactivamos la escritura de longs (4 bytes).
  outportb(VGA_CRTC_INDEX, VGA_UNDERLINE_LOC);
  outportb(VGA_CRTC_DATA, 0x00);

  // Activamos la escritura de bytes (1 byte).
  outportb(VGA_CRTC_INDEX, VGA_MODE_CONTROL);
  outportb(VGA_CRTC_DATA, 0xe3);
}

void video_unchained_set_plane_mask(uint8_t plane)
{
  outportb(VGA_SC_INDEX, VGA_MAP_MASK);
  outportb(VGA_SC_DATA, 1 << (plane & VGA_PLANE_MASK));
}

void video_put_pixel(uint16_t x, uint16_t y, uint8_t color) {
  video_buffer[y * VIDEO_WIDTH + x] = color;
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
