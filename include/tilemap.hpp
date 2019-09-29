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

  tile() {
  }

  tile(ALLEGRO_BITMAP *bm, float tx = 0, float ty = 0) {
    this->bitmap = bm;
    this->tx = tx;
    this->ty = ty;
  }
};

tile create_colored_tile(int w, int h, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
  tile t(al_create_bitmap(w, h), 0, 0);
  al_set_target_bitmap(t.bitmap);
  al_clear_to_color(al_map_rgba(r, g, b, a));
  return t;
}

class tilemap {
public:
  int width, height;
  int tile_size;
  std::vector<std::vector<tile*>> tiles;
  tile *empty;

  tilemap() {
  }

  tilemap(int w, int h, int ts, tile *e) {
    this->create(w, h, ts, e);
  }

  void create(int w, int h, int ts, tile *e) {
    this->width = w;
    this->height = h;
    this->tile_size = ts;
    this->empty = e;

    for (int i = 0; i < h; i++) {
      std::vector<tile*> v;
      for(int j = 0; j < w; j++) {
        v.push_back(e);
      }
      this->tiles.push_back(v);
    }
  }

  void add(tile *t, int r, int c) {
    this->tiles[r][c] = t;
  }

  void draw(float ox, float oy, float scale_factor) {
  for (int row = 0; row < this->tiles.size(); row++)
    for (int column = 0; column < this->tiles[row].size(); column++)
      complex_draw_bitmap(this->tiles[row][column]->bitmap,
			  this->tile_size * this->tiles[row][column]->tx, // sx
			  this->tile_size * this->tiles[row][column]->ty, // sy
			  this->tile_size, this->tile_size, // sw, sh
			  al_map_rgb(255,255,255), // tint
			  0.0, 0.0, // cx, cy
			  ox + this->tile_size * column * scale_factor, // dx
			  oy + this->tile_size * row * scale_factor, //dy
			  scale_factor, scale_factor, // xscale, yscale
			  0, 0); // angle, flags
  }
};

#endif
