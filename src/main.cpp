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

#include <vector>
#include <algorithm>
#include <cstdio>

#include <SDL.h>

#include <emscripten.h>

#include "../include/constants.hpp"
#include "../include/direction.hpp"
#include "../include/materials.hpp"
#include "../include/tilemap.hpp"
#include "../include/level.hpp"

struct expcontext {
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool loop, held;
  direction dir;
  level* l;
};

void main_loop(void* arg) {
  struct expcontext* c = (struct expcontext*) arg;

  if (c->loop) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_KEYDOWN) {
	if (!(c->held)) {
	  switch (e.key.keysym.sym) {
	  case SDLK_UP:
	  case SDLK_w:
	    c->dir = NORTH;
	    break;
	  case SDLK_RIGHT:
	  case SDLK_d:
	    c->dir = EAST;
	    break;
	  case SDLK_DOWN:
	  case SDLK_s:
	    c->dir = SOUTH;
	    break;
	  case SDLK_LEFT:
	  case SDLK_a:
	    c->dir = WEST;
	    break;
	  case SDLK_r:
	    c->l->reload();
	  }

	  if (c->dir >= 0) {
	    c->held = true;
	  }
	}
      }
      else if (e.type == SDL_KEYUP)  {
	if (c->held)
	  c->held = false;
      }
      else if (e.type == SDL_WINDOWEVENT_RESIZED) {
	c->l->justify(e.window.data1, e.window.data2);
      }
      else if (e.type == SDL_QUIT) {
	c->loop = false;
      }
    }

    if (c->held && c->dir != STATIC) {
      c->l->move_player(c->dir);
      c->dir = STATIC;
    }

    SDL_SetRenderDrawColor(c->renderer, 0, 0, 0, 255);    
    SDL_RenderClear(c->renderer);
    c->l->draw();

    SDL_SetRenderDrawColor(c->renderer, 255, 0, 0, 255);
    
    SDL_RenderPresent(c->renderer);
  } else {
      SDL_DestroyRenderer(c->renderer);
      SDL_DestroyWindow(c->window);
      c->l->clear();

      emscripten_cancel_main_loop();
  }
}

int main(int argc, char **argv) { 
  SDL_Window *window; SDL_Renderer *renderer;
 
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Unable to initialize SDL2: %s\n", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow("Explorers",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    DISPLAY_WIDTH,
			    DISPLAY_HEIGHT,
			    0);

  if (window == NULL) {
    SDL_Log("Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  renderer = SDL_CreateRenderer(window, -1,
				SDL_RENDERER_ACCELERATED |
				SDL_RENDERER_TARGETTEXTURE);

  if (renderer == NULL) {
    SDL_Log("Could not create renderer: %s\n", SDL_GetError());
    return 1;
  }
  
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  init_materials(renderer);

  SDL_SetRenderTarget(renderer, NULL);

  level l(renderer, "assets/1.lv");
  l.justify(DISPLAY_WIDTH, DISPLAY_HEIGHT);

  struct expcontext c = {.window = window,
			 .renderer = renderer,
			 .loop = true,
			 .held = false,
			 .dir = STATIC,
			 .l = &l};
  emscripten_set_main_loop_arg(main_loop, (void*) &c, 0, true);
}
