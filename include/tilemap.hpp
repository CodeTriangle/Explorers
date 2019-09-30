#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>

#include <allegro5/allegro.h>

#include "constants.hpp"

#define complex_draw_bitmap al_draw_tinted_scaled_rotated_bitmap_region

class tile {
public:
  ALLEGRO_BITMAP *bitmap;
  float tx, ty;
  int size;
  
  tile() {
  }

  tile(ALLEGRO_BITMAP *bm, float x, float y, int size) {
    bitmap = bm;
    tx = x;
    ty = y;
    size = size;
  }

  tile(int s, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    bitmap = al_create_bitmap(s, s);
    tx = 0;
    ty = 0;
    size = s;
    al_set_target_bitmap(bitmap);
    al_clear_to_color(al_map_rgba(r, g, b, a));
  }

  void draw(int x, int y, int scale_factor) {
    complex_draw_bitmap(bitmap,
			size * tx, // sx
			size * ty, // sy
			size, size, // sw, sh
			al_map_rgb(255,255,255), // tint
			0.0, 0.0, // cx, cy
			x, y, //dx, dy
			scale_factor, scale_factor, // xscale, yscale
			0, 0); // angle, flags
  }
};

class tilemap {
public:
  int width, height;
  int tile_size;
  std::vector<std::vector<tile*>> tiles;
  tile *empty;

  tilemap() {
  }

  tilemap(int w, int h, int ts, tile *e) {
    create(w, h, ts, e);
  }

  void create(int w, int h, int ts, tile *e) {
    width = w;
    height = h;
    tile_size = ts;
    empty = e;

    for (int i = 0; i < h; i++) {
      std::vector<tile*> v;
      for(int j = 0; j < w; j++) {
        v.push_back(e);
      }
      tiles.push_back(v);
    }
  }

  tile *contents(int r, int c) {
    return tiles[r][c];
  }

  void add(tile *t, int r, int c) {
    tiles[r][c] = t;
  }

  void remove(int r, int c) {
    tiles[r][c] = empty;
  }

  void draw(float ox, float oy, float scale_factor) {
  for (int row = 0; row < tiles.size(); row++)
    for (int column = 0; column < tiles[row].size(); column++)
      tiles[row][column]->draw(ox + tile_size * column * scale_factor,
			       oy + tile_size * row * scale_factor,
			       scale_factor);
  }
};

#endif
