#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <fstream>

#include <allegro5/allegro.h>

#include "tilemap.hpp"
#include "materials.hpp"

class level {
public:
  int width, height;
  int enterd, exitd;
  int enterp, exitp;
  int scale_factor;
  int origin_x, origin_y;
  int player_r, player_c;
  tilemap back, fore, rubble;
  bool future, travel;
  bool done;
  
  level(std::string fn) {
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
          if (t[0] != NULL)
            back.add(t[0], i, j);
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
      player_r = enterp;
      player_c = width;
    }
    else if (enterd == 2) {
      player_r = height;
      player_c = enterp;
    }
    else if (enterd == 3)  {
      player_r = enterp;
      player_c = 0;
    }

    fore.add(&MATERIALS["PLAYER"], player_r, player_c);
  }

  void justify(int dw, int dh) {
    scale_factor = std::min(dw / (8 * 16), dh / (6 * 16));
    origin_x = dw / 2 - width * 16 * scale_factor / 2;
    origin_y = dh / 2 - height * 16 * scale_factor / 2; 
  }

  bool move_tile(int r, int c, int d) {
    int target_r = r, target_c = c;
    bool can_move = false, check_done = false;
    tile* to_move = fore.contents(r, c);
    if (d == 0) {
      if (r == 1)
        check_done = true;
      else
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
      if (d%2 == 0 && c == exitp ||
	  d%2 == 1 && r == exitp) {
	if (future)
	  done = true;
	else
	  future = true;
      }
      return false;
    }

    if (target_r == r && target_c == c)
      return false;

    if (is_collidable(fore.contents(target_r, target_c)) ||
	is_collidable(back.contents(target_r, target_c)))
      return false;
    else
      can_move = true;

    if (future) {
      can_move = false;
      if(is_collidable(rubble.contents(target_r, target_c)))
	return false;
      else
	can_move = true;
    }
    
    if (fore.contents(target_r, target_c) == &MATERIALS["BLOCK"]) {
      can_move = false;
      if(move_tile(target_r, target_c, d))
	can_move = true;
    }

    if (can_move) {
      fore.add(fore.contents(r, c), target_r, target_c);
      if (to_move == &MATERIALS["PLAYER"]) {
	player_r = target_r;
	player_c = target_c;
      }
      fore.remove(r, c);
      return true;
    }
    else
      return false;
  }

  bool move_player(int d) {
    return move_tile(player_r, player_c, d);
  }

  void draw() {
    back.draw(origin_x, origin_y, scale_factor);
    if (future)
      rubble.draw(origin_x, origin_y, scale_factor);
    fore.draw(origin_x, origin_y, scale_factor);
    
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
};
#endif
