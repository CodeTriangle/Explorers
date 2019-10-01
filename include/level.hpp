#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <allegro5/allegro.h>

#include "tilemap.hpp"
#include "materials.hpp"

class level {
public:
  int id;
  int width, height;
  int enterd, exitd;
  int enterp, exitp;
  int scale_factor;
  int origin_x, origin_y;
  int player_or, player_oc;
  int player_r, player_c;
  int buttons, buttons_down;
  tilemap back, fore, rubble;
  bool future, travel;
  bool done;
  
  level(std::string fn) {
    id = 1;
    load(fn);
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
    enterd = a[2];
    enterp = a[3];
    exitd  = a[4];
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

  bool move_tile(int r, int c, int d) {
    int target_r = r, target_c = c;
    bool can_move = false, check_done = false;
    if (d == 0) {
      if (r == 1) {
        check_done = true;
	if (future && rubble.contents(r-1,c) == &MATERIALS["VINE"])
	  target_r--;
      }
      else if (r != 0)
	target_r--;
    }
    else if (d == 1) {
      if (c == width - 1)
	check_done = true;
      else
	target_c++;
    }
    else if (d == 2) {
      if (r == height - 1)
	check_done = true;
      else
	target_r++;
    }
    else if (d == 3) {
      if (c == 0)
	check_done = true;
      else
	target_c--;
    }

    if (check_done) {
      if (d == exitd &&
	  ((d%2 == 0 && c == exitp) ||
	   (d%2 == 1 && r == exitp + 1))) {
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
    }

    if (target_r == r && target_c == c)
      return false;

    tile* back_here = back.contents(r, c);
    tile* fore_here = fore.contents(r, c);
    tile* rubble_here = rubble.contents(r, c);
    tile* back_there = back.contents(target_r, target_c);
    tile* fore_there = fore.contents(target_r, target_c);
    tile* rubble_there = rubble.contents(target_r, target_c);

    if (is_collidable(fore_there) ||
	is_collidable(back_there))
      can_move = false;
    else
      can_move = true;

    if (future) {
      can_move = false;
      if(is_collidable(rubble_there))
	can_move = false;
      else
	can_move = true;
    }
    
    if (fore_there == &MATERIALS["BLOCK"]) {
      can_move = false;
      if(move_tile(target_r, target_c, d))
	can_move = true;
    }

    if ((d == 0 && rubble.contents(r-1, c) == &MATERIALS["VINE"]) ||
	(d == 1 && rubble.contents(r, c+1) == &MATERIALS["VINE"]) ||
	(d == 3 && rubble.contents(r, c-1) == &MATERIALS["VINE"])) 
      can_move = true;

    if (can_move) {
      fore.remove(r, c);
      bool should_add = false;
      
      if (back_there != &MATERIALS["PIT"] &&
	  back_there != &MATERIALS["HOLE"] &&
	  back_there != &MATERIALS["BRIDB"])
	should_add = true;

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

  bool move_player(int d) {
    return move_tile(player_r, player_c, d);
  }

  void draw() {
    if (id < 6) {
      back.draw(origin_x, origin_y, scale_factor);
      if (future)
	rubble.draw(origin_x, origin_y, scale_factor);
      fore.draw(origin_x, origin_y, scale_factor);

      if (future) TEXTS[0].draw(origin_x, origin_y - 32, 2);
      else TEXTS[1].draw(origin_x, origin_y - 32, 2);

      if (id == 1)
	TEXTS[2].draw(origin_x, origin_y - 16, 2);
      else
	TEXTS[3].draw(origin_x, origin_y - 16, 2);
    
      // I was in a hurry so this code is not very consise
    
      if (enterp != -1) {
	if (enterd == 0)
	  MATERIALS["DOOR"].draw(origin_x + enterp * TILE_SIZE * scale_factor,
				 origin_y,
				 scale_factor);
	else if (enterd == 1)
	  MATERIALS["GROUND"].draw(origin_x + width * TILE_SIZE * scale_factor,
				   origin_y + (enterp + 1) * TILE_SIZE * scale_factor,
				   scale_factor);
	else if (enterd == 2)
	  MATERIALS["GROUND"].draw(origin_x + enterp * TILE_SIZE * scale_factor,
				   origin_y + height * TILE_SIZE * scale_factor,
				   scale_factor);
	else if (enterd == 3)
	  MATERIALS["GROUND"].draw(origin_x - TILE_SIZE * scale_factor,
				   origin_y + (enterp + 1) * TILE_SIZE * scale_factor,
				   scale_factor);
      }

      if (exitp != -1) {
	if (exitd == 0)
	  MATERIALS["DOOR"].draw(origin_x + exitp * TILE_SIZE * scale_factor,
				 origin_y,
				 scale_factor);
	else if (exitd == 1)
	  MATERIALS["GROUND"].draw(origin_x + width * TILE_SIZE * scale_factor,
				   origin_y + (exitp + 1) * TILE_SIZE * scale_factor,
				   scale_factor);
	else if (exitd == 2)
	  MATERIALS["GROUND"].draw(origin_x + exitp * TILE_SIZE * scale_factor,
				   origin_y + height * TILE_SIZE * scale_factor,
				   scale_factor);
	else if (exitd == 3)
	  MATERIALS["GROUND"].draw(origin_x - TILE_SIZE * scale_factor,
				   origin_y + (exitp + 1) * TILE_SIZE * scale_factor,
				   scale_factor);
      }
    }
    else
      TEXTS[4].draw(origin_x, origin_y, width * TILE_SIZE * scale_factor / 80);
  }
};
#endif
