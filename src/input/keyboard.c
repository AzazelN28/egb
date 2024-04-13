#include "keyboard.h"

volatile bool keys[KEYBOARD_SIZE];
static _go32_dpmi_seginfo prev_handler, new_handler;

static void keyboard_handler() {
  uint8_t code = inportb(KEYBOARD_CODE);
  if (code & KEYBOARD_BREAK) {
    keys[code & (KEYBOARD_BREAK - 1)] = false;
  } else {
    keys[code] = true;
  }
  outportb(0x20, 0x20);
}

bool keyboard_start() {
  _go32_dpmi_get_protected_mode_interrupt_vector(
    KEYBOARD_INTERRUPT,
    &prev_handler
  );
  new_handler.pm_offset = (uint32_t)keyboard_handler;
  new_handler.pm_selector = _go32_my_cs();
  _go32_dpmi_allocate_iret_wrapper(&new_handler);
  _go32_dpmi_set_protected_mode_interrupt_vector(
    KEYBOARD_INTERRUPT,
    &new_handler
  );
  return true;
}

void keyboard_stop() {
  if (_go32_dpmi_set_protected_mode_interrupt_vector(
    KEYBOARD_INTERRUPT,
    &prev_handler
  ) == -1) {
    return;
  } else {
    _go32_dpmi_free_iret_wrapper(&new_handler);
  }
}

bool key_is_pressed(uint8_t key) {
  return keys[key];
}

bool key_is_released(uint8_t key) {
  return !keys[key];
}
