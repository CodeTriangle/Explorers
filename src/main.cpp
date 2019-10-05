#include <vector>
#include <algorithm>
#include <cstdio>

#include <SDL.h>

#include "../include/tilemap.hpp"
#include "../include/constants.hpp"
#include "../include/materials.hpp"
#include "../include/level.hpp"

int main(int argc, char **argv) {
  SDL_Window *window;
  SDL_Renderer *renderer;

  int scale_factor, origin_x, origin_y;
  
  bool loop = true, redraw = true;

  bool held = false, should_move = false;

  int current_level = 0;

  int direction = -1;

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

  while (loop) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_KEYDOWN) {
	if (!held) {

	  switch (e.key.keysym.sym) {
	  case SDLK_UP:
	  case SDLK_w:
	    direction = 0;
	    break;
	  case SDLK_RIGHT:
	  case SDLK_d:
	    direction = 1;
	    break;
	  case SDLK_DOWN:
	  case SDLK_s:
	    direction = 2;
	    break;
	  case SDLK_LEFT:
	  case SDLK_a:
	    direction = 3;
	    break;
	  case SDLK_r:
	    l.reload();
	  }

	  if (direction >= 0) {
	    held = true;
	  }
	}
      }
      else if (e.type == SDL_KEYUP)  {
	if (held)
	  held = false;
      }
      else if (e.type == SDL_WINDOWEVENT_RESIZED) {
	l.justify(e.window.data1, e.window.data2);
      }
      else if (e.type == SDL_QUIT) {
	loop = false;
      }
    }

    if (held && direction >= 0) {
      l.move_player(direction);
      direction = -1;
    }
    
    SDL_RenderClear(renderer);
    l.draw();
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  l.clear();
}
