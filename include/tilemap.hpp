#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>

#include <allegro5/allegro.h>

#include "constants.hpp"

#define complex_draw_bitmap al_draw_tinted_scaled_rotated_bitmap_region

typedef struct {
  ALLEGRO_BITMAP *bitmap;
  float tx, ty;
} tile;

tile create_tile(ALLEGRO_BITMAP *bm, float tx = 0, float ty = 0) {
  return (tile) {.bitmap = bm, .tx = tx, .ty = ty};
}

void destroy_tile(tile *t) {
  al_destroy_bitmap(t->bitmap);
  free(t);
}

tile create_colored_tile(int w, int h, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
  tile t = {.bitmap = al_create_bitmap(w, h), .tx = 0, .ty = 0};
  al_set_target_bitmap(t.bitmap);
  al_clear_to_color(al_map_rgba(r, g, b, a));
  return t;
}

typedef struct {
  int width, height;
  int tile_size;
  std::vector<std::vector<tile*>> tiles;
  tile *empty;
} tilemap;

tilemap create_tilemap(int w, int h, int ts, tile *e) {
  tilemap tm;
  
  tm.width = w;
  tm.height = h;
  tm.tile_size = ts;
  tm.empty = e;

  for (int i = 0; i < h; i++) {
    std::vector<tile*> v;
    for(int j = 0; j < w; j++) {
      v.push_back(e);
    }
    tm.tiles.push_back(v);
  }
  
  return tm;
}

bool add_tile_to_tilemap(tile *t, tilemap *tm, int r = -1, int c = -1) {
  if (r == -1 && c == -1) {
    for (int row = 0; row < tm->tiles.size(); row++)
      for (int column = 0; column < tm->tiles[row].size(); column++)
	if (tm->tiles[row][column] == tm->empty) {
	  tm->tiles[row][column] = t;
	  return true;
	}
    return false;
  }
  else if (r == -1 || c == -1)
    return false;
  else
    tm->tiles[r][c] = t;
  return true;
}

void draw_tilemap(tilemap *tm, float scale_factor) {
  for (int row = 0; row < tm->tiles.size(); row++)
    for (int column = 0; column < tm->tiles[row].size(); column++)
      complex_draw_bitmap(tm->tiles[row][column]->bitmap,
			  tm->tile_size * tm->tiles[row][column]->tx, // sx
			  tm->tile_size * tm->tiles[row][column]->ty, // sy
			  tm->tile_size, tm->tile_size, // sw, sh
			  al_map_rgb(255,255,255), // tint
			  0, 0, // cx, cy
			  tm->tile_size * column * scale_factor, // dx
			  tm->tile_size * row * scale_factor, //dy
			  scale_factor, scale_factor, // xscale, yscale
			  0, 0); // angle, flags
}

#endif
