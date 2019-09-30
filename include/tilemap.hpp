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

  tile(ALLEGRO_BITMAP *bm, float tx, float ty, int size) {
    this->bitmap = bm;
    this->tx = tx;
    this->ty = ty;
    this->size = size;
  }

  tile(int size, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    this->bitmap = al_create_bitmap(size, size);
    this->tx = 0;
    this->ty = 0;
    this->size = size;
    al_set_target_bitmap(this->bitmap);
    al_clear_to_color(al_map_rgba(r, g, b, a));
  }

  void draw(int x, int y, int scale_factor) {
    complex_draw_bitmap(this->bitmap,
			this->size * this->tx, // sx
			this->size * this->ty, // sy
			this->size, this->size, // sw, sh
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
      this->tiles[row][column]->draw(ox + this->tile_size * column * scale_factor,
				     oy + this->tile_size * row * scale_factor,
				     scale_factor);
  }
};

#endif
