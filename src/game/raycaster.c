#include "raycaster.h"

column_t columns[VIDEO_WIDTH] = {0};
view_t view = {0};
ray_t ray = {0};
rows_t rows = {0};

fixed_t xs[VIDEO_WIDTH] = {0};
fixed_t row_distances[VIDEO_HALF_HEIGHT] = {0};
uint8_t checker_texture[RAYCASTER_TEXTURE_BUFFER_SIZE] = {0};

void raycaster_start()
{
  view.changed_position = true;
  view.changed_rotation = true;
#ifdef RAYCASTER_XOR_TEXTURE
  for (int y = 0; y < RAYCASTER_TEXTURE_SIZE; y++)
  {
    for (int x = 0; x < RAYCASTER_TEXTURE_SIZE; x++)
    {
      uint8_t c = x ^ y;
      checker_texture[y * RAYCASTER_TEXTURE_SIZE + x] = c;
    }
  }
#else
  uint8_t fg = 0x03, bg = 0x05, num_cols = 4, num_rows = 4, col = 0, row = 0;
  for (int y = 0; y < RAYCASTER_TEXTURE_SIZE; y++)
  {
    row = (y / (RAYCASTER_TEXTURE_SIZE / num_rows));
    for (int x = 0; x < RAYCASTER_TEXTURE_SIZE; x++)
    {
      col = (x / (RAYCASTER_TEXTURE_SIZE / num_cols));
      checker_texture[y * RAYCASTER_TEXTURE_SIZE + x] = (row + col) % 2 ? fg : bg;
    }
  }
#endif

  // Precalculamos todas las distancias de las filas.
  for (int8_t y = 0; y < VIDEO_HALF_HEIGHT; y++)
  {
    int8_t p = VIDEO_HALF_HEIGHT - y;
    row_distances[y] = FIXED_DIV(RAYCASTER_VIDEO_FIXED_HALF_HEIGHT, FIXED_FROM_INT(p));
  }

  // Precalculamos los valores de x.
  for (int16_t x = 0; x < VIDEO_WIDTH; x++)
  {
    xs[x] = (FIXED_FROM_INT(2 * x) / VIDEO_WIDTH) - FIXED_UNIT;
  }
}

void raycaster_render()
{
  if (view.changed_rotation) {
    rows.start.x = view.direction.x - view.plane.x;
    rows.start.y = view.direction.y - view.plane.y;
    rows.end.x = view.direction.x + view.plane.x;
    rows.end.y = view.direction.y + view.plane.y;

    rows.delta.x = (rows.end.x - rows.start.x);
    rows.delta.y = (rows.end.y - rows.start.y);

    rows.step.x = FIXED_DIV(rows.delta.x, RAYCASTER_VIDEO_FIXED_WIDTH);
    rows.step.y = FIXED_DIV(rows.delta.y, RAYCASTER_VIDEO_FIXED_WIDTH);
  }

  for (int16_t x = VIDEO_WIDTH - 1; x >= 0; --x)
  {
    rows.mult_step.x = rows.step.x * x;
    rows.mult_step.y = rows.step.y * x;

    ray.x = xs[x];

    ray.direction.x = view.direction.x + FIXED_MUL(view.plane.x, ray.x);
    ray.direction.y = view.direction.y + FIXED_MUL(view.plane.y, ray.x);

    ray.delta_dist.x = ray.direction.x == 0 ? MAXINT : FIXED_ABS(FIXED_DIV(FIXED_UNIT, ray.direction.x));
    ray.delta_dist.y = ray.direction.y == 0 ? MAXINT : FIXED_ABS(FIXED_DIV(FIXED_UNIT, ray.direction.y));

    ray.tile.x = FIXED_FLOOR(view.position.x);
    ray.tile.y = FIXED_FLOOR(view.position.y);

    if (ray.direction.x < 0)
    {
      ray.step.x = RAYCASTER_NEG_ONE;
      ray.tile_delta.x = (view.position.x - FIXED_FLOOR(view.position.x));
    }
    else
    {
      ray.step.x = RAYCASTER_POS_ONE;
      ray.tile_delta.x = (FIXED_FLOOR(view.position.x) + FIXED_UNIT - view.position.x);
    }

    if (ray.direction.y < 0)
    {
      ray.step.y = RAYCASTER_NEG_ONE;
      ray.tile_delta.y = (view.position.y - FIXED_FLOOR(view.position.y));
    }
    else
    {
      ray.step.y = RAYCASTER_POS_ONE;
      ray.tile_delta.y = (FIXED_FLOOR(view.position.y) + FIXED_UNIT - view.position.y);
    }

    ray.side_dist.x = FIXED_MUL(ray.tile_delta.x, ray.delta_dist.x);
    ray.side_dist.y = FIXED_MUL(ray.tile_delta.y, ray.delta_dist.y);

    ray.hit = false;
    ray.iterations = 0;
    while (ray.hit == false && ray.iterations < RAYCASTER_MAX_ITERATIONS)
    {
      ray.iterations++;
      if (ray.side_dist.x < ray.side_dist.y)
      {
        ray.side_dist.x += ray.delta_dist.x;
        ray.tile.x += ray.step.x;
        ray.side = 0;
      }
      else
      {
        ray.side_dist.y += ray.delta_dist.y;
        ray.tile.y += ray.step.y;
        ray.side = 1;
      }

      ray.tilei.x = FIXED_TO_INT(ray.tile.x);
      ray.tilei.y = FIXED_TO_INT(ray.tile.y);

      if (ray.tilei.x < 0 || ray.tilei.y < 0 || ray.tilei.x >= MAP_WIDTH || ray.tilei.y >= MAP_HEIGHT)
      {
        ray.side = 2; // OUTSIDE.
        ray.hit = true;
      }
      else if (map[ray.tilei.x][ray.tilei.y] > 0)
      {
        ray.hit = true;
      }
    }

    if (ray.side == 0)
    {
      ray.perp_wall_dist = (ray.side_dist.x - ray.delta_dist.x);
      ray.wall_x = view.position.y + FIXED_MUL(ray.perp_wall_dist, ray.direction.y);
      ray.wall_x -= FIXED_FLOOR(ray.wall_x);
      columns[x].tex_x = FIXED_MUL(ray.wall_x, RAYCASTER_TEXTURE_FIXED_SIZE);
      if (ray.direction.x < 0)
      {
        columns[x].tex_x = RAYCASTER_TEXTURE_FIXED_SIZE - columns[x].tex_x - FIXED_UNIT;
      }
    }
    else
    {
      ray.perp_wall_dist = (ray.side_dist.y - ray.delta_dist.y);
      ray.wall_x = view.position.x + FIXED_MUL(ray.perp_wall_dist, ray.direction.x);
      ray.wall_x -= FIXED_FLOOR(ray.wall_x);
      columns[x].tex_x = FIXED_MUL(ray.wall_x, RAYCASTER_TEXTURE_FIXED_SIZE);
      if (ray.direction.y < 0)
      {
        columns[x].tex_x = RAYCASTER_TEXTURE_FIXED_SIZE - columns[x].tex_x - FIXED_UNIT;
      }
    }

    columns[x].u = FIXED_ABS(FIXED_TO_INT(columns[x].tex_x));
    columns[x].tile.x = ray.tilei.x;
    columns[x].tile.y = ray.tilei.y;
    columns[x].side = ray.side;
    columns[x].z = ray.perp_wall_dist;
    if (columns[x].z == 0)
      columns[x].z = FIXED_UNIT;

    columns[x].fix_height = FIXED_DIV(RAYCASTER_VIDEO_FIXED_HEIGHT, columns[x].z);
    columns[x].height = FIXED_TO_INT(columns[x].fix_height);
    columns[x].half_height = columns[x].height >> 1;
    columns[x].inc_y = FIXED_DIV(RAYCASTER_TEXTURE_FIXED_SIZE, columns[x].fix_height);
    columns[x].tex_y = 0;

    columns[x].draw_start = columns[x].draw_end = VIDEO_HALF_HEIGHT;
    columns[x].draw_start += -columns[x].half_height;
    if (columns[x].draw_start < RAYCASTER_MIN_Y)
    {
      //columns[x].tex_y += FIXED_MUL(FIXED_FROM_INT((RAYCASTER_MIN_Y - columns[x].draw_start)), columns[x].inc_y);
      columns[x].tex_y += (RAYCASTER_MIN_Y - columns[x].draw_start) * columns[x].inc_y;
      columns[x].draw_start = RAYCASTER_MIN_Y;
    }

    columns[x].draw_end += columns[x].half_height;
    if (columns[x].draw_end > RAYCASTER_MAX_Y)
    {
      columns[x].draw_end = RAYCASTER_MAX_Y;
    }

    if (!view.floor_ceil_hidden)
    {
      for (uint8_t y = RAYCASTER_MIN_Y; y < columns[x].draw_start; y++)
      {
        if (row_distances[y] > RAYCASTER_MAX_DRAWING_Z)
          continue;

        fixed_t floor_step_x = FIXED_MUL(row_distances[y], rows.mult_step.x);
        fixed_t floor_step_y = FIXED_MUL(row_distances[y], rows.mult_step.y);

        fixed_t floor_x = view.position.x + floor_step_x + FIXED_MUL(row_distances[y], rows.start.x);
        fixed_t floor_y = view.position.y + floor_step_y + FIXED_MUL(row_distances[y], rows.start.y);

        uint8_t u = FIXED_TO_INT(RAYCASTER_TEXTURE_SIZE * (floor_x - FIXED_FLOOR(floor_x))) & (RAYCASTER_TEXTURE_SIZE - 1);
        uint8_t v = FIXED_TO_INT(RAYCASTER_TEXTURE_SIZE * (floor_y - FIXED_FLOOR(floor_y))) & (RAYCASTER_TEXTURE_SIZE - 1);

        // TECHO
        uint8_t color = checker_texture[RAYCASTER_TEXTURE_SIZE * v + u];
        VIDEO_PUT_PIXEL(x, y, color);

        // SUELO
        if ((VIDEO_HEIGHT - y - 1) < RAYCASTER_MAX_Y)
          VIDEO_PUT_PIXEL(x, (VIDEO_HEIGHT - y - 1), color);
      }
    }

    if (columns[x].z > RAYCASTER_MAX_DRAWING_Z)
      continue;

    // PAREDES
    // TODO: Utilizar el truco de Carmack en DOOM de que las texturas están
    //       volteadas 90º. Así podremos hacer algo del estilo de
    //       checker_texture[RAYCASTER_TEXTURE_SIZE * columns[x].u + columns[x].v]
    for (uint8_t y = columns[x].draw_start; y < VIDEO_HALF_HEIGHT; y++)
    {
      columns[x].v = FIXED_TO_INT(columns[x].tex_y);
      columns[x].tex_y += columns[x].inc_y;
      uint8_t color = checker_texture[RAYCASTER_TEXTURE_SIZE * columns[x].v + columns[x].u];

      VIDEO_PUT_PIXEL(x, y, color);

      columns[x].v = FIXED_TO_INT(RAYCASTER_TEXTURE_FIXED_SIZE - columns[x].tex_y);
      color = checker_texture[RAYCASTER_TEXTURE_SIZE * columns[x].v + columns[x].u];
      VIDEO_PUT_PIXEL(x, (VIDEO_HEIGHT - y - 1), color);
    }
  }

  view.changed_position = false;
  view.changed_rotation = false;
}
