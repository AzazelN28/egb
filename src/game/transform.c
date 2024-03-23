#include "transform.h"

bool transform_are_adjacent(const transform_t *a, const transform_t *b)
{
  return (a->position.x == b->position.x && abs(a->position.y - b->position.y) == 1) || (a->position.y == b->position.y && abs(a->position.x - b->position.x) == 1);
}

bool transform_are_facing(const transform_t *a, const transform_t *b)
{
  if (!transform_are_adjacent(a, b)) {
    return false;
  }
  if (a->position.x == b->position.x) {
    if (a->position.y < b->position.y) {
      return a->direction == DIR_EAST && b->direction == DIR_WEST;
    }
    return a->direction == DIR_WEST && b->direction == DIR_EAST;
  } else if (a->position.y == b->position.y) {
    if (a->position.x < b->position.x) {
      return a->direction == DIR_SOUTH && b->direction == DIR_NORTH;
    }
    return a->direction == DIR_NORTH && b->direction == DIR_SOUTH;
  }
  return false;
}
