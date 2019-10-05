#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>

#include <SDL.h>

#include "constants.hpp"

class tile {
public:
  SDL_Texture *texture;
  SDL_Rect rect;
  int width;
  int height;
  
  tile() {
  }

  tile(SDL_Texture *tex, int x1, int y1, int size) {
    texture = tex;
    rect = { .x = x1, .y = y1, .w = size, .h = size };
  }

  tile(SDL_Texture *tex, int x1, int y1, int w1, int h1) {
    texture = tex;
    rect = { .x = x1, .y = y1, .w = w1, .h = h1 };
  }

  tile(SDL_Renderer *renderer,
       unsigned char r, unsigned char g, unsigned char b, unsigned char a,
       int w1, int h1) {
    texture = SDL_CreateTexture(renderer,
				SDL_PIXELFORMAT_RGBA8888,
				SDL_TEXTUREACCESS_TARGET,
				w1, h1);
    rect = { .x = 0, .y = 0, .w = w1, .h = h1 };
    
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
    SDL_RenderFillRect(renderer, &rect);
  }

  void draw(SDL_Renderer *renderer, int x1, int y1, int scale_factor) {
    const SDL_Rect dstrect = {.x = x1, .y = y1,
			      .w = width * scale_factor, .h = height * scale_factor };
    
    SDL_RenderCopy(renderer, texture, &rect, &dstrect);
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

  void clear() {
    tiles.clear();
  }

  void replace(tile *from, tile *to) {
    for (int i = 0; i < tiles.size(); i++)
      for (int j = 0; j < tiles[i].size(); j++)
	if (tiles[i][j] == from)
	  tiles[i][j] = to;
  }

  void draw(SDL_Renderer *renderer, float ox, float oy, float scale_factor) {
  for (int row = 0; row < tiles.size(); row++)
    for (int column = 0; column < tiles[row].size(); column++)
      tiles[row][column]->draw(renderer,
			       ox + tile_size * column * scale_factor,
			       oy + tile_size * row * scale_factor,
			       scale_factor);
  }
};

#endif
