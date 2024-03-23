#include "speaker.h"

void speaker_beep(uint32_t frequency, uint32_t duration) {
  sound(frequency);
  sleep(duration);
  sound(0);
}
