#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <fstream>

#include <allegro5/allegro.h>

#include "tilemap.hpp"
#include "materials.hpp"

class level {
public:
  int width, height;
  int enter, exit;
  int scale_factor;
  int origin_x, origin_y;
  tilemap back, fore, rubble;
  
  level(std::string fn) {
    std::ifstream ls(fn);
  
    int a[4];
  
    for (int i = 0; i < 4; i++) {
      char numbers[4];
      ls.getline(numbers, 4, ',');
      a[i] = atoi(numbers);
    }

    this->width  = a[0];
    this->height = a[1]+1;
    this->enter  = a[2];
    this->exit   = a[3];
  
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
  }

  void justify(int dw, int dh) {
    scale_factor = std::min(dw / (8 * 16), dh / (6 * 16));
    origin_x = dw / 2 - this->width * 16 * scale_factor / 2;
    origin_y = dh / 2 - this->height * 16 * scale_factor / 2; 
  }

  void draw() {
    this->back.draw(origin_x, origin_y, scale_factor);
    this->fore.draw(origin_x, origin_y, scale_factor);
    this->rubble.draw(origin_x, origin_y, scale_factor);
  }
};
#endif
