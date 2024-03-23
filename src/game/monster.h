#pragma once

#include <stdint.h>

#include "transform.h"
#include "stats.h"

typedef struct monster_ {
  transform_t transform;
  stats_t stats;
} monster_t;
