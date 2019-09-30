#ifndef TILES_HPP
#define TILES_HPP

#include <fstream>
#include <map>
#include <array>
#include <string>
#include <cstdlib>

#include <allegro5/allegro.h>

#include "tilemap.hpp"

std::map<std::string, tile> MATERIALS;
std::map<char, std::array<tile*, 3>> CHARS;

void init_materials() {
  std::ifstream ms("assets/materials");

  while (ms.good()) {
    char name[7], func[3];
    ms.getline(name, 7, ' ');
    ms.getline(func, 3, ':');

    if (std::string(func) == "ct") {
      unsigned char a[4];
      
      for (int i = 0; i < 4; i++) {
	char c[4];
	ms.getline(c, 5, ',');
	a[i] = (unsigned char) atoi(c);
      }

      tile t = create_colored_tile(16, 16, a[0], a[1], a[2], a[3]);
      
      MATERIALS.insert(std::pair<std::string, tile>(std::string(name), t));
      
      ms.get();
    }
  }
  ms.close();
  
  ms.open("assets/chars");

  while (ms.good()) {
    char name[2];
    ms.getline(name, 2, ' ');

    std::array<tile*, 3> a;

    for (int i = 0; i < 3; i++) {
      char m[7];
      ms.getline(m, 7, ',');
      if (std::string(m) == "")
	a[i] = &MATERIALS["EMPTY"];
      else
	a[i] = &MATERIALS[std::string(m)];
    }

    CHARS.insert(std::make_pair(name[0], a));

    ms.get();
  }
}

#endif
