#include "mouse.h"

mouse_t mouse = {0};

bool mouse_start() {
  __dpmi_regs r;
  r.x.ax = MOUSE_INIT;
  if (__dpmi_int(MOUSE_INTERRUPT, &r) == -1)
    return false;

  if (r.x.ax == 0xFFFF)
    mouse.available = false;
  else if (r.x.ax == 0x0000)
  {
    mouse.available = true;
    mouse.num_buttons = r.x.bx;
  }
  return true;
}

void mouse_show() {
  __dpmi_regs r;
  r.x.ax = MOUSE_SHOW;
  __dpmi_int(MOUSE_INTERRUPT, &r);
}

void mouse_hide() {
  __dpmi_regs r;
  r.x.ax = MOUSE_HIDE;
  __dpmi_int(MOUSE_INTERRUPT, &r);
}

void mouse_get_status() {
  if (!mouse.available) {
    return;
  }

  __dpmi_regs r;
  r.x.ax = MOUSE_GET_STATUS;
  __dpmi_int(MOUSE_INTERRUPT, &r);

  // NOTA: Los valores de x e y son de 0 a 639 y de 0 a 199 respectivamente
  // independientemente del modo de vídeo.
  vec2i_copy(&mouse.previous, &mouse.current);
  vec2i_set(&mouse.current, (r.x.cx >> 1), r.x.dx);
  vec2i_sub(&mouse.delta, &mouse.current, &mouse.previous);

  mouse.buttons = r.x.bx;
  mouse.left = (r.x.bx & MOUSE_LEFT_BUTTON_MASK) ? true : false;
  mouse.right = (r.x.bx & MOUSE_RIGHT_BUTTON_MASK) ? true : false;
}

void mouse_stop() {}

bool mouse_is_pressed(uint8_t button) {
  return (mouse.buttons & button) ? true : false;
}

bool mouse_is_released(uint8_t button) {
  return (mouse.buttons & button) ? false : true;
}
