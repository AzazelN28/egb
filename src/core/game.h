#pragma once

#include "../input/keyboard.h"
#include "../input/mouse.h"
#include "../video/palette.h"
#include "../video/video.h"
#include "../video/font.h"
#include "../video/fade.h"
#include "../sound/speaker.h"
#include "../sound/sb.h"
#include "../math/fixed.h"
#include "../game/map.h"
#include "../game/entity.h"
#include "../game/colormap.h"
#include "../game/raycaster.h"
#include "../game/player.h"
#include "./timer.h"
#include "./log.h"
#include <stdbool.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>

extern bool game_is_running;

typedef struct game_options_
{
  bool dump_defaults;
} game_options_t;

void game_get_options(game_options_t *options, int argc, char** argv);
bool game_init(game_options_t *options);
void game_loop();
void game_quit();
