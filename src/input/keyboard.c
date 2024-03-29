#include "keyboard.h"

uint8_t keys[KEYBOARD_SIZE];

static _go32_dpmi_seginfo prev_handler, new_handler;

void keyboard_handler() {
  uint8_t code, state, is_pressed = 1;
  code = inportb(KEYBOARD_CODE);
  state = inportb(KEYBOARD_STATE);
  if (code & KEYBOARD_BREAK) {
    is_pressed = 0;
    code -= KEYBOARD_BREAK;
  } else {
    is_pressed = 1;
  }
  keys[code] = is_pressed;
}

void keyboard_start() {
  _go32_dpmi_get_protected_mode_interrupt_vector(KEYBOARD_INTERRUPT,
                                                 &prev_handler);

  new_handler.pm_offset = (uint32_t)keyboard_handler;
  new_handler.pm_selector = _go32_my_cs();

  _go32_dpmi_chain_protected_mode_interrupt_vector(KEYBOARD_INTERRUPT,
                                                   &new_handler);
}

void keyboard_stop() {
  _go32_dpmi_set_protected_mode_interrupt_vector(8, &prev_handler);
}

bool key_is_pressed(uint8_t key) {
  return keys[key];
}

bool key_is_released(uint8_t key) {
  return !keys[key];
}
