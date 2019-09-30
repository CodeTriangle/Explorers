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
  
  level(std::string fn) {
    std::ifstream ls(fn);
  
    int a[6];
  
    for (int i = 0; i < 6; i++) {
      char numbers[4];
      ls.getline(numbers, 4, ',');
      a[i] = atoi(numbers);
    }

    this->width  = a[0];
    this->height = a[1]+1;
    this->enterd = a[2];
    this->enterp = a[3];
    this->exitd  = a[4];
    this->exitp  = a[5];
    this->future = false;
    this->travel = false;
  
    this->back.create(this->width,this->height,16,&MATERIALS["EMPTY"]);
    this->fore.create(this->width,this->height,16,&MATERIALS["EMPTY"]);
    this->rubble.create(this->width,this->height,16,&MATERIALS["EMPTY"]);
  
    ls.get();
  
    for (int i = 0; i < this->height; i++) {
      char row[7];
      ls.getline(row, 7);
      for (int j = 0; j < this->width; j++) {
        if (row[j] == '\0')
          break;
        else {
          std::array<tile*, 3> t = CHARS[row[j]];
          if (t[0] != NULL)
            this->back.add(t[0], i, j);
          if (t[1] != NULL)
            this->fore.add(t[1], i, j);
          if (t[2] != NULL)
            this->rubble.add(t[2], i, j);
        }
      }
    }

    if (enterp == -1) {
      this->player_r = 2;
      this->player_c = 2;
    }
    else if (enterd == 0) {
      this->player_r = 1;
      this->player_c = enterp;
    }
    else if (enterd == 1) {
      this->player_r = enterp;
      this->player_c = this->width;
    }
    else if (enterd == 2) {
      this->player_r = this->height;
      this->player_c = enterp;
    }
    else if (enterd == 3)  {
      this->player_r = enterp;
      this->player_c = 0;
    }

    this->fore.add(&MATERIALS["PLAYER"], player_r, player_c);
  }

  void justify(int dw, int dh) {
    scale_factor = std::min(dw / (8 * 16), dh / (6 * 16));
    origin_x = dw / 2 - this->width * 16 * scale_factor / 2;
    origin_y = dh / 2 - this->height * 16 * scale_factor / 2; 
  }

  void move_tile(int r, int c, int d) {
  }

  void move_player(int d) {
    int target_r = player_r, target_c = player_c;
    bool can_move = false;
    if (d == 0 && player_r != 0)
      target_r--;
    else if (d == 1 && player_c != this->width - 1)
      target_c++;
    else if (d == 2 && player_r != this->height - 1)
      target_r++;
    else if (d == 3 && player_c != 0)
      target_c--;

    if (fore.contents(target_r, target_c) == &MATERIALS["EMPTY"] &&
	back.contents(target_r, target_c) == &MATERIALS["GROUND"]) {
      can_move = true;
    }

    if (can_move) {
      fore.remove(player_r, player_c);
      player_r = target_r;
      player_c = target_c;
      fore.add(&MATERIALS["PLAYER"], player_r, player_c);
    }
  }

  void draw() {
    this->back.draw(origin_x, origin_y, scale_factor);
    if (future)
      this->rubble.draw(origin_x, origin_y, scale_factor);
    this->fore.draw(origin_x, origin_y, scale_factor);
    
    // I was in a hurry so this code is not very consise
    
    if (enterp != -1) {
      if (enterd == 0)
	MATERIALS["DOOR"].draw(origin_x + enterp * TILE_SIZE * scale_factor,
				 origin_y,
				 this->scale_factor);
      else if (enterd == 1)
	MATERIALS["GROUND"].draw(origin_x + this->width * TILE_SIZE * scale_factor,
				 origin_y + (enterp + 1) * TILE_SIZE * scale_factor,
				 this->scale_factor);
      else if (enterd == 2)
	MATERIALS["GROUND"].draw(origin_x + enterp * TILE_SIZE * scale_factor,
				 origin_y + this->height * TILE_SIZE * scale_factor,
				 scale_factor);
      else if (enterd == 3)
	MATERIALS["GROUND"].draw(origin_x - TILE_SIZE * scale_factor,
				 origin_y + (enterp + 1) * TILE_SIZE * scale_factor,
				 this->scale_factor);
    }

    if (exitp != -1) {
      if (exitd == 0)
	MATERIALS["DOOR"].draw(origin_x + exitp * TILE_SIZE * scale_factor,
				 origin_y,
				 this->scale_factor);
      else if (exitd == 1)
	MATERIALS["GROUND"].draw(origin_x + this->width * TILE_SIZE * scale_factor,
				 origin_y + (exitp + 1) * TILE_SIZE * scale_factor,
				 this->scale_factor);
      else if (exitd == 2)
	MATERIALS["GROUND"].draw(origin_x + exitp * TILE_SIZE * scale_factor,
				 origin_y + this->height * TILE_SIZE * scale_factor,
				 scale_factor);
      else if (exitd == 3)
	MATERIALS["GROUND"].draw(origin_x - TILE_SIZE * scale_factor,
				 origin_y + (exitp + 1) * TILE_SIZE * scale_factor,
				 this->scale_factor);
    }
  }
};
#endif
