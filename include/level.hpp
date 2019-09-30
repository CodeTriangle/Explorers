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

    if (enterp == -1)
      this->fore.add(&MATERIALS["PLAYER"], 2, 2);
    else if (enterd == 0)
      this->fore.add(&MATERIALS["PLAYER"], 1, enterp);
    else if (enterd == 1)
      this->fore.add(&MATERIALS["PLAYER"], enterp, this->width);
    else if (enterd == 2)
      this->fore.add(&MATERIALS["PLAYER"], this->height, enterp);
    else if (enterd == 3)
      this->fore.add(&MATERIALS["PLAYER"], enterp, 0);
  }

  void justify(int dw, int dh) {
    scale_factor = std::min(dw / (8 * 16), dh / (6 * 16));
    origin_x = dw / 2 - this->width * 16 * scale_factor / 2;
    origin_y = dh / 2 - this->height * 16 * scale_factor / 2; 
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
