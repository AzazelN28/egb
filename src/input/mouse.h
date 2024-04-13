#pragma once

#include <dpmi.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../math/vec2.h"

#define MOUSE_INIT 0x00
#define MOUSE_SHOW 0x01
#define MOUSE_HIDE 0x02
#define MOUSE_GET_STATUS 0x03
#define MOUSE_INTERRUPT 0x33

#define MOUSE_LEFT_BUTTON_MASK 0x01
#define MOUSE_RIGHT_BUTTON_MASK 0x02

typedef struct mouse_ {
  vec2int16_t current;
  vec2int16_t previous;
  vec2int16_t delta;
  int8_t num_buttons;
  int8_t buttons;
  bool left;
  bool right;
  bool available;
} mouse_t;

extern mouse_t mouse;

bool mouse_start();
void mouse_show();
void mouse_hide();
void mouse_get_status();
void mouse_stop();
bool mouse_is_pressed(uint8_t button);
bool mouse_is_released(uint8_t button);
