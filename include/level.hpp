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

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <SDL.h>

#include "direction.hpp"
#include "materials.hpp"
#include "tilemap.hpp"


#ifdef __cplusplus
extern "C" {
#endif

class level {
public:
  int id;
  int width, height;
  direction enterd, exitd;
  int enterp, exitp;
  int scale_factor;
  int origin_x, origin_y;
  int player_or, player_oc;
  int player_r, player_c;
  int buttons, buttons_down;
  tilemap back, fore, rubble;
  bool future, travel;
  bool done;
  SDL_Renderer *renderer;
  
  level(SDL_Renderer *r, std::string fn) {
    id = 1;
    renderer = r;
    load(fn);
    renderer = r;
  }

  void load(std::string fn) {
    std::ifstream ls(fn);
  
    int a[6];
  
    for (int i = 0; i < 6; i++) {
      char numbers[4];
      ls.getline(numbers, 4, ',');
      a[i] = atoi(numbers);
    }

    width  = a[0];
    height = a[1]+1;
    enterd = (direction) a[2];
    enterp = a[3];
    exitd  = (direction) a[4];
    exitp  = a[5];
    
    future = false;
    travel = false;
    done   = false;
    
    buttons = 0;
    buttons_down = 0;
    
    back.create(width,height,16,&MATERIALS["EMPTY"]);
    fore.create(width,height,16,&MATERIALS["EMPTY"]);
    rubble.create(width,height,16,&MATERIALS["EMPTY"]);
  
    ls.get();
  
    for (int i = 0; i < height; i++) {
      char row[7];
      ls.getline(row, 7);
      for (int j = 0; j < width; j++) {
        if (row[j] == '\0')
          break;
        else {
          std::array<tile*, 3> t = CHARS[row[j]];
          if (t[0] != NULL) {
	    if (t[0] == &MATERIALS["BUTTON"])
	      buttons++;
            back.add(t[0], i, j);
	  }
          if (t[1] != NULL)
            fore.add(t[1], i, j);
          if (t[2] != NULL)
            rubble.add(t[2], i, j);
        }
      }
    }

    if (enterp == -1) {
      player_r = 2;
      player_c = 2;
    }
    else if (enterd == 0) {
      player_r = 1;
      player_c = enterp;
    }
    else if (enterd == 1) {
      player_r = enterp + 1;
      player_c = width - 1;
    }
    else if (enterd == 2) {
      player_r = height;
      player_c = enterp;
    }
    else if (enterd == 3)  {
      player_r = enterp + 1;
      player_c = 0;
    }

    player_or = player_r;
    player_oc = player_c;

    fore.add(player(flip(enterd), future), player_r, player_c);
  }

  void reload() {
    clear();
    char a[12];
    sprintf(a, "assets/%d.lv", id);
    load(a);
  }

  void clear() {
    back.clear();
    fore.clear();
    rubble.clear();
  }

  void justify(int dw, int dh) {
    scale_factor = std::min(dw / (8 * 16), dh / (6 * 16));
    origin_x = dw / 2 - width * 16 * scale_factor / 2;
    origin_y = dh / 2 - height * 16 * scale_factor / 2; 
  }

  void travel_to_future() {
    if (travel) {
      future = true;
      fore.remove(player_r, player_c);
      player_r = player_or;
      player_c = player_oc;
      fore.add(player(flip(enterd), future), player_r, player_c);
      back.replace(&MATERIALS["BRIDGE"], &MATERIALS["BRIDB"]);
    }
  }

  void travel_to_past() {
    if (travel) {
      future = false;
      fore.remove(player_r, player_c);
      player_r = player_or;
      player_c = player_oc;
      fore.add(player(flip(enterd), future), player_r, player_c);
    }
  }

  bool move_tile(int r, int c, direction d) {
    int target_r = r, target_c = c;

    coords_next_to(r, c, d, &target_r, &target_c);

    if ((target_r < 1 || target_r >= height ||
	 target_c < 0 || target_c >= width) &&
	((is_vertical(d) && c == exitp) ||
	 (is_horizontal(d) && r == exitp + 1)) &&
	d == exitd) {
      if (future) {
	id++;
	if (id < 6)
	  reload();
	else
	  clear();
      }
      else {
	travel = true;
	travel_to_future();
      }
      return false;
    }

    if (target_r < 0 || target_r >= height ||
	target_c < 0 || target_c >= width)
      return false;

    tile* back_here = back.contents(r, c);
    tile* fore_here = fore.contents(r, c);
    tile* rubble_here = rubble.contents(r, c);
    tile* back_there = back.contents(target_r, target_c);
    tile* fore_there = fore.contents(target_r, target_c);
    tile* rubble_there = rubble.contents(target_r, target_c);

    bool can_move = true;

    if (is_collidable(fore_there) ||
	is_collidable(back_there) ||
	(future && is_collidable(rubble_there)))
      can_move = false;
    
    if (fore_there == &MATERIALS["BLOCK"]) {
      can_move = false;
      if(move_tile(target_r, target_c, d))
	can_move = true;
    }

    if (rubble_there == &MATERIALS["VINE"] &&
	0 <= target_c < width &&
	future)
      can_move = true;

    if (can_move) {
      fore.remove(r, c);
      bool should_add = true;
      
      if (back_there == &MATERIALS["PIT"] ||
	  back_there == &MATERIALS["HOLE"] ||
	  back_there == &MATERIALS["BRIDB"])
	should_add = false;

      if (back_here == &MATERIALS["BUTTD"]) {
	back.add(&MATERIALS["BUTTON"], r, c);
	buttons_down--;
      }
      
      if (back_there == &MATERIALS["BUTTON"]) {
	back.add(&MATERIALS["BUTTD"], target_r, target_c);
	buttons_down++;
      }
      
      if (is_player(fore_here)) {
	player_r = target_r;
	player_c = target_c;

	if (back_here == &MATERIALS["BROKEN"])
	  back.add(&MATERIALS["HOLE"], r, c);
      }

      if (fore_here == &MATERIALS["BLOCK"] &&
	  back_there == &MATERIALS["BROKEN"]) {
	should_add = false;
	back.add(&MATERIALS["HOLE"], target_r, target_c);
      }

      if (should_add) {
	if (is_player(fore_here))
	  fore.add(player(d, future), target_r, target_c);
	else
	  fore.add(fore_here, target_r, target_c);
      }

      if (buttons_down == buttons &&
	  buttons != 0)
	back.replace(&MATERIALS["SWITCH"], &MATERIALS["SWITP"]);
      
      return true;
    }
    else
      return false;
  }

  bool move_player(direction d) {
    return move_tile(player_r, player_c, d);
  }

  void draw() {
    if (id < 6) {
      back.draw(renderer, origin_x, origin_y, scale_factor);
      if (future)
	rubble.draw(renderer, origin_x, origin_y, scale_factor);
      fore.draw(renderer, origin_x, origin_y, scale_factor);

      if (future) TEXTS[0].draw(renderer, origin_x, origin_y - 32, 2);
      else TEXTS[1].draw(renderer, origin_x, origin_y - 32, 2);

      if (id == 1)
	TEXTS[2].draw(renderer, origin_x, origin_y - 16, 2);
      else
	TEXTS[3].draw(renderer, origin_x, origin_y - 16, 2);
    
      // I was in a hurry so this code is not very consise
    
      if (enterp != -1) {
	if (enterd == 0)
	  MATERIALS["DOOR"].draw(renderer,
				 origin_x + enterp * TILE_SIZE * scale_factor,
				 origin_y,
				 scale_factor);
	else if (enterd == 1)
	  MATERIALS["GROUND"].draw(renderer,
				   origin_x + width * TILE_SIZE * scale_factor,
				   origin_y + (enterp + 1) * TILE_SIZE * scale_factor,
				   scale_factor);
	else if (enterd == 2)
	  MATERIALS["GROUND"].draw(renderer,
				   origin_x + enterp * TILE_SIZE * scale_factor,
				   origin_y + height * TILE_SIZE * scale_factor,
				   scale_factor);
	else if (enterd == 3)
	  MATERIALS["GROUND"].draw(renderer,
				   origin_x - TILE_SIZE * scale_factor,
				   origin_y + (enterp + 1) * TILE_SIZE * scale_factor,
				   scale_factor);
      }

      if (exitp != -1) {
	if (exitd == 0)
	  MATERIALS["DOOR"].draw(renderer,
				 origin_x + exitp * TILE_SIZE * scale_factor,
				 origin_y,
				 scale_factor);
	else if (exitd == 1)
	  MATERIALS["GROUND"].draw(renderer,
				   origin_x + width * TILE_SIZE * scale_factor,
				   origin_y + (exitp + 1) * TILE_SIZE * scale_factor,
				   scale_factor);
	else if (exitd == 2)
	  MATERIALS["GROUND"].draw(renderer,
				   origin_x + exitp * TILE_SIZE * scale_factor,
				   origin_y + height * TILE_SIZE * scale_factor,
				   scale_factor);
	else if (exitd == 3)
	  MATERIALS["GROUND"].draw(renderer,
				   origin_x - TILE_SIZE * scale_factor,
				   origin_y + (exitp + 1) * TILE_SIZE * scale_factor,
				   scale_factor);
      }
    }
    else
      TEXTS[4].draw(renderer, origin_x, origin_y, width * TILE_SIZE * scale_factor / 80);
  }
};


#ifdef __cplusplus
}
#endif

#endif
