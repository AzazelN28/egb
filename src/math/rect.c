#include "rect.h"

rect_t *rect_new(int32_t x, int32_t y, int32_t w, int32_t h) {
  rect_t *rect = calloc(1, sizeof(rect_t));
  assert(rect != NULL);
  return rect_set(rect, x, y, w, h);
}

void rect_free(rect_t *rect) {
  assert(rect != NULL);
  free(rect);
}

rect_t *rect_set(rect_t *r, int32_t x, int32_t y, int32_t w, int32_t h) {
  point_set(&r->position, x, y);
  point_set(&r->size, w, h);
  return r;
}

rect_t *rect_copy(rect_t *out, rect_t *r) {
  return rect_set(out, r->position.x, r->position.y, r->size.x, r->size.y);
}

rect_t *rect_clone(rect_t *r) {
  return rect_new(r->position.x, r->position.y, r->size.x, r->size.y);
}

bool rect_contains(rect_t *r, int32_t x, int32_t y) {
  return x >= r->position.x && x < r->position.x + r->size.x &&
         y >= r->position.y && y < r->position.y + r->size.y;
}

bool rect_contains_point(rect_t *r, point_t *v) {
  return rect_contains(r, v->x, v->y);
}

bool rect_intersects(rect_t *a, rect_t *b) {
  if (a->position.x + a->size.x < b->position.x)
    return false;
  if (a->position.x > b->position.x + b->size.x)
    return false;
  if (a->position.y + a->size.y < b->position.y)
    return false;
  if (a->position.y > b->position.y + b->size.y)
    return false;
  return true;
}

rect_t *rect_intersection(rect_t *out, rect_t *a, rect_t *b) {
  out->position.x = max(a->position.x, b->position.x);
  out->position.y = max(a->position.y, b->position.y);
  out->size.x = max(min(a->position.x + a->size.x, b->position.x + b->size.x) -
                        out->position.x,
                    0);
  out->size.y = max(min(a->position.y + a->size.y, b->position.y + b->size.y) -
                        out->position.y,
                    0);
  return out;
}
