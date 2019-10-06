/* BSD 3-Clause License
 * 
 * Copyright (C) 2019, Reuben Staley/CodeTriangle
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>

#include <SDL.h>

#include "constants.hpp"

class tile {
public:
  SDL_Texture *texture;
  SDL_Rect rect;
  
  tile() {
    texture = NULL;
  }

  tile(SDL_Texture *tex, int x1, int y1, int size) {
    texture = tex;
    rect = { .x = x1 * size, .y = y1 * size, .w = size, .h = size };
  }

  tile(SDL_Texture *tex, int x1, int y1, int w1, int h1) {
    texture = tex;
    rect = { .x = x1 * w1, .y = y1 * h1, .w = w1, .h = h1 };
  }

  void draw(SDL_Renderer *renderer, int x1, int y1, int scale_factor) {
    if (texture == NULL)
      return;
    
    const SDL_Rect dstrect = {.x = x1, .y = y1,
			      .w = rect.w * scale_factor, .h = rect.h * scale_factor };
    
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
