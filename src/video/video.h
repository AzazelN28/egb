#pragma once

#include <dpmi.h>
#include <pc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/nearptr.h>

// #define VIDEO_ENABLE_UNCHAINED
#define VIDEO_ENABLE_CLEAR
// #define VIDEO_ENABLE_VSYNC

#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 200
#define VIDEO_HALF_WIDTH 160
#define VIDEO_HALF_HEIGHT 100

#ifdef VIDEO_ENABLE_UNCHAINED
#define VIDEO_PLANE_WIDTH 80
#define VIDEO_PAGES 4
#endif

#define VIDEO_BUFFER_SIZE 64000

#define VIDEO_INTERRUPT 0x10
#define VIDEO_INPUT_STATUS 0x3DA
#define VIDEO_VERTICAL_RETRACE 0x08
#define VIDEO_VGA_ADDRESS 0xA0000

#define VIDEO_MODE_320x200 0x13
#define VIDEO_MODE_TEXT 0x03

/* VGA sequence controller */
#define VGA_SC_INDEX 0x03c4
#define VGA_SC_DATA 0x03c5

/* VGA CRT controller */
#define VGA_CRTC_INDEX 0x03d4
#define VGA_CRTC_DATA 0x03d5

#define VGA_MAP_MASK 0x02
#define VGA_PLANE_MASK 0x03
#define VGA_ALL_PLANES 0xFF02
#define VGA_MEMORY_MODE 0x04

#define VGA_UNDERLINE_LOC 0x14
#define VGA_MODE_CONTROL 0x17

#define VIDEO_PUT_PIXEL(x, y, c) video_buffer[y * VIDEO_WIDTH + x] = c

extern uint8_t *video_ptr;
extern uint8_t video_buffer[VIDEO_BUFFER_SIZE];

bool video_start();
void video_stop();
bool video_set(uint32_t mode);
void video_render();

void video_unchained_enable();
void video_unchained_clear();
void video_unchained_set_plane_mask(uint8_t plane);

void video_put_pixel(uint16_t x, uint16_t y, uint8_t color);
void video_put_pixel_safe(uint16_t x, uint16_t y, uint8_t color);
uint8_t video_get_pixel(uint16_t x, uint16_t y);
void video_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
void video_draw_line(uint16_t ax, uint16_t ay, uint16_t bx, uint16_t by, uint8_t color);
