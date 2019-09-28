#ifndef TILES_HPP
#define TILES_HPP

#include <fstream>
#include <map>
#include <string>
#include <cstdlib>

#include <allegro5/allegro.h>

#include "tilemap.hpp"

std::map<std::string, tile> MATERIALS;
std::map<char, tile*[3]> CHARS;

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
      
      MATERIALS.insert(std::pair<std::string, tile>(std::string(name),
						    create_colored_tile(16, 16,
									a[0], a[1],
								        a[2], a[3])));
      ms.get();
    }
  }

  ms.close();
}

#endif
