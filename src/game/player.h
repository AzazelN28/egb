#pragma once

#include <stdint.h>

#include "transform.h"
#include "stats.h"

typedef struct player_ {
  transform_t transform;
  stats_t stats;
} player_t;

