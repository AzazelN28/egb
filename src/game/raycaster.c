#include "raycaster.h"

struct whatever_ {
  fixed_t x, y;
  fixed_t step_x, step_y;
  bool needs_update;
} whatever[VIDEO_HALF_HEIGHT] = {0};

column_t columns[VIDEO_WIDTH] = {0};
view_t view = {0};
ray_t ray = {0};
rows_t rows = {0};

fixed_t xs[VIDEO_WIDTH] = {0};
fixed_t row_distances[VIDEO_HALF_HEIGHT] = {0};
bool visited_map[MAP_WIDTH][MAP_HEIGHT] = {false};

/**
 * @brief Inicializa el motor de raycasting.
 *
 * Precalcula las distancias de las filas y los valores de x para
 * cada columna y cada fila.
 */
void raycaster_start()
{
  view.changed_position = true;
  view.changed_rotation = true;

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

/**
 * @brief Renderiza el mundo.
 */
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

  if (view.changed_rotation || view.changed_position)
  {
    view.inv_det = FIXED_DIV(FIXED_UNIT, (FIXED_MUL(view.plane.x, view.direction.y) - FIXED_MUL(view.direction.x, view.plane.y)));
    for (uint8_t y = 0; y < VIDEO_HALF_HEIGHT; y++)
    {
      whatever[y].x = view.position.x + FIXED_MUL(rows.start.x, row_distances[y]);
      whatever[y].y = view.position.y + FIXED_MUL(rows.start.y, row_distances[y]);

      whatever[y].step_x = FIXED_MUL(row_distances[y], rows.step.x);
      whatever[y].step_y = FIXED_MUL(row_distances[y], rows.step.y);
    }
  }

  // Limpiamos el mapa de visitados.
  memset(visited_map, 0, sizeof(visited_map));

  for (int16_t x = VIDEO_WIDTH - 1; x >= 0; --x)
  {
    ray.x = xs[x];

    ray.direction.x = view.direction.x + FIXED_MUL(view.plane.x, ray.x);
    ray.direction.y = view.direction.y + FIXED_MUL(view.plane.y, ray.x);

    ray.delta_dist.x = ray.direction.x == 0 ? MAXINT : FIXED_ABS(FIXED_DIV(FIXED_UNIT, ray.direction.x));
    ray.delta_dist.y = ray.direction.y == 0 ? MAXINT : FIXED_ABS(FIXED_DIV(FIXED_UNIT, ray.direction.y));

    ray.tile.x = FIXED_FLOOR(view.position.x);
    ray.tile.y = FIXED_FLOOR(view.position.y);

    ray.tilei.x = FIXED_TO_INT(ray.tile.x);
    ray.tilei.y = FIXED_TO_INT(ray.tile.y);

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
    while (ray.hit == false
        && ray.iterations < RAYCASTER_MAX_ITERATIONS)
    {
      ray.iterations++;
      // Comprobamos si en este tile existen entidades.
      if (adjacent_entities[ray.tilei.x][ray.tilei.y] != NULL && visited_map[ray.tilei.x][ray.tilei.y] == false)
      {
        // Aquí marcamos la casilla como que ha sido
        // visitada, y ya no tenemos que volver a comprobar
        // si hay entidades en ella.
        visited_map[ray.tilei.x][ray.tilei.y] = true;

        // Recorremos todas las entidades que hay en esta casilla.
        entity_t *current = adjacent_entities[ray.tilei.x][ray.tilei.y];
        while (current != NULL)
        {
          fixed_t delta_x = current->position.x - view.position.x;
          fixed_t delta_y = current->position.y - view.position.y;
          if (FIXED_ABS(delta_x) <= FIXED_HALF_UNIT
           && FIXED_ABS(delta_y) <= FIXED_HALF_UNIT) {
            current = current->next_adjacent;
            continue;
          }

          current->transform_x = FIXED_MUL(view.inv_det, (FIXED_MUL(view.direction.y, delta_x) - FIXED_MUL(view.direction.x, delta_y)));
          current->transform_y = FIXED_MUL(view.inv_det, (-FIXED_MUL(view.plane.y, delta_x) + FIXED_MUL(view.plane.x, delta_y)));
          current->x = VIDEO_HALF_WIDTH * (FIXED_UNIT + FIXED_DIV(current->transform_x, current->transform_y));
          current->y = RAYCASTER_VIDEO_FIXED_HALF_HEIGHT;
          current->z = FIXED_MUL(delta_x, delta_x) + FIXED_MUL(delta_y, delta_y);

          fixed_t screen_size = FIXED_ABS(FIXED_DIV(RAYCASTER_VIDEO_FIXED_HEIGHT, current->transform_y));
          current->inc = FIXED_DIV(TEXTURE_FIXED_SIZE, screen_size);
          current->screen_size = FIXED_TO_INT(screen_size);
          current->screen_half_size = (current->screen_size >> 1);

          current->texture_start.y = 0;
          current->screen_start.y = -current->screen_half_size + VIDEO_HALF_HEIGHT;
          if (current->screen_start.y < RAYCASTER_MIN_Y)
          {
            current->texture_start.y = FIXED_MUL(current->inc, FIXED_FROM_INT(RAYCASTER_MIN_Y - current->screen_start.y));
            current->screen_start.y = RAYCASTER_MIN_Y;
          }

          current->screen_end.y = current->screen_half_size + VIDEO_HALF_HEIGHT;
          if (current->screen_end.y > RAYCASTER_MAX_Y)
          {
            current->screen_end.y = RAYCASTER_MAX_Y;
          }

          int16_t x = FIXED_TO_INT(current->x);
          current->texture_start.x = 0;
          current->screen_start.x = -current->screen_half_size + x;
          if (current->screen_start.x < 0)
          {
            current->texture_start.x = FIXED_MUL(current->inc, FIXED_FROM_INT(0 - current->screen_start.x));
            current->screen_start.x = 0;
          }

          current->screen_end.x = current->screen_half_size + x;
          if (current->screen_end.x > VIDEO_WIDTH)
          {
            current->screen_end.x = VIDEO_WIDTH;
          }
          entity_add_visible(current);
          current = current->next_adjacent;
        }
      }

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
      else if (map.tiles.data[ray.tilei.x][ray.tilei.y] > 0)
      {
        ray.hit = true;
      }
    }

    if (ray.side == 0)
    {
      ray.perp_wall_dist = (ray.side_dist.x - ray.delta_dist.x);
      ray.wall_x = view.position.y + FIXED_MUL(ray.perp_wall_dist, ray.direction.y);
      ray.wall_x -= FIXED_FLOOR(ray.wall_x);
      columns[x].tex_x = ray.wall_x * TEXTURE_SIZE;
      if (ray.direction.x < 0)
      {
        columns[x].tex_x = TEXTURE_FIXED_SIZE - columns[x].tex_x - FIXED_UNIT;
      }
    }
    else
    {
      ray.perp_wall_dist = (ray.side_dist.y - ray.delta_dist.y);
      ray.wall_x = view.position.x + FIXED_MUL(ray.perp_wall_dist, ray.direction.x);
      ray.wall_x -= FIXED_FLOOR(ray.wall_x);
      columns[x].tex_x = ray.wall_x * TEXTURE_SIZE;
      if (ray.direction.y < 0)
      {
        columns[x].tex_x = TEXTURE_FIXED_SIZE - columns[x].tex_x - FIXED_UNIT;
      }
    }

    columns[x].u = FIXED_ABS(FIXED_TO_INT(columns[x].tex_x));
    columns[x].tile.x = ray.tilei.x;
    columns[x].tile.y = ray.tilei.y;
    columns[x].side = ray.side;
    columns[x].z = ray.perp_wall_dist;
    if (columns[x].z == 0)
    {
      columns[x].z = FIXED_UNIT;
    }
    columns[x].fix_height = FIXED_DIV(RAYCASTER_VIDEO_FIXED_HEIGHT, columns[x].z);
    columns[x].height = FIXED_TO_INT(columns[x].fix_height);
    columns[x].half_height = columns[x].height >> 1;
    columns[x].inc_y = TEXTURE_FIXED_SIZE / columns[x].height;
    columns[x].tex_y = 0;
    columns[x].texture = map.tiles.data[ray.tilei.x][ray.tilei.y];

    columns[x].shade = FIXED_TO_INT(columns[x].z << COLORMAP_SHADE_MULTIPLIER) & COLORMAP_SHADE_MASK;

    columns[x].draw_start = columns[x].draw_end = VIDEO_HALF_HEIGHT;
    columns[x].draw_start += -columns[x].half_height;
    if (columns[x].draw_start < RAYCASTER_MIN_Y)
    {
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
        {
          continue;
        }

        fixed_t floor_step_x = whatever[y].step_x * x;
        fixed_t floor_step_y = whatever[y].step_y * x;

        fixed_t floor_x = floor_step_x + whatever[y].x;
        fixed_t floor_y = floor_step_y + whatever[y].y;

        uint8_t u = FIXED_TO_INT(TEXTURE_SIZE * FIXED_FRACT(floor_x)) & (TEXTURE_SIZE - 1);
        uint8_t v = FIXED_TO_INT(TEXTURE_SIZE * FIXED_FRACT(floor_y)) & (TEXTURE_SIZE - 1);

        // TECHO
        uint8_t shade = FIXED_TO_INT(row_distances[y] << COLORMAP_SHADE_MULTIPLIER) & COLORMAP_SHADE_MASK;
        uint16_t uv = TEXTURE_SIZE * v + u;
        uint8_t color_c = textures[map.textures.ceiling & TEXTURE_MASK][uv];
        if (!(map.textures.ceiling & TEXTURE_NONE))
        {
          VIDEO_PUT_PIXEL(x, y, colormap[shade][color_c]);
        }

        // SUELO
        if ((VIDEO_HEIGHT - y - 1) < RAYCASTER_MAX_Y)
        {
          uint8_t color_f = textures[map.textures.floor & TEXTURE_MASK][uv];
          if (!(map.textures.floor & TEXTURE_NONE))
          {
            VIDEO_PUT_PIXEL(x, (VIDEO_HEIGHT - y - 1), colormap[shade][color_f]);
          }
        }
      }
    }

    if (columns[x].z > RAYCASTER_MAX_DRAWING_Z)
      continue;

    // PAREDES
    // TODO: Utilizar el truco de Carmack en DOOM de que las texturas están
    //       volteadas 90º. Así podremos hacer algo del estilo de
    //       checker_texture[TEXTURE_SIZE * columns[x].u + columns[x].v]
    for (uint8_t y = columns[x].draw_start; y < VIDEO_HALF_HEIGHT; y++)
    {
      columns[x].v = FIXED_TO_INT(columns[x].tex_y);
      columns[x].tex_y += columns[x].inc_y;
      uint32_t texture = columns[x].texture;

      uint8_t color = textures[texture][TEXTURE_SIZE * columns[x].v + columns[x].u];
      VIDEO_PUT_PIXEL(x, y, colormap[columns[x].shade][color]);

      columns[x].v = FIXED_TO_INT(TEXTURE_FIXED_SIZE - columns[x].tex_y);
      color = textures[texture][TEXTURE_SIZE * columns[x].v + columns[x].u];

      VIDEO_PUT_PIXEL(x, (VIDEO_HEIGHT - y - 1), colormap[columns[x].shade][color]);
    }
  }

  // Renderizamos las entidades visibles.
  entity_t *current = visible_entities;
  while (current != NULL)
  {
    // Si la profundidad de la entidad es mayor que la
    // distancia de dibujado, entonces pasamos a la
    // siguiente entidad visible.
    if (current->transform_y >= RAYCASTER_MAX_DRAWING_Z
     || current->transform_y <= RAYCASTER_MIN_DRAWING_Z)
    {
      current = current->next_visible;
      continue;
    }

    fixed_t u = current->texture_start.x;
    fixed_t v = current->texture_start.y;
    for (int16_t y = current->screen_start.y; y < current->screen_end.y; y++)
    {
      int16_t tv = FIXED_TO_INT(v) & (TEXTURE_SIZE - 1);
      uint32_t texture_offset = TEXTURE_SIZE * tv;
      for (int16_t x = current->screen_start.x; x < current->screen_end.x; x++)
      {
        if (current->transform_y > columns[x].z)
        {
          u += current->inc;
          continue;
        }

        int16_t tu = FIXED_TO_INT(u) & (TEXTURE_SIZE - 1);

        uint8_t shade = FIXED_TO_INT(current->transform_y << COLORMAP_SHADE_MULTIPLIER) & COLORMAP_SHADE_MASK;
        uint8_t color = checker_texture[texture_offset + tu];
        if (color == TRANSPARENT_COLOR)
        {
          u += current->inc;
          continue;
        }

        VIDEO_PUT_PIXEL(x, y, colormap[shade][color]);
        u += current->inc;
      }
      v += current->inc;
      u = current->texture_start.x;
    }

    num_visible_entities++;
    current = current->next_visible;
  }

  view.changed_position = false;
  view.changed_rotation = false;
}
