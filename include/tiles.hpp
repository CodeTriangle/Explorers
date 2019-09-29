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

void read_level(tilemap *b, tilemap *f, tilemap *r, std::string fn) {
  std::ifstream ls(fn);

  int a[4];

  for (int i = 0; i < 4; i++) {
    char numbers[4];
    ls.getline(numbers, 4, ',');
    a[i] = atoi(numbers);
  }

  a[1]++;

  b->create(a[0],a[1],16,&MATERIALS["EMPTY"]);
  f->create(a[0],a[1],16,&MATERIALS["EMPTY"]);
  r->create(a[0],a[1],16,&MATERIALS["EMPTY"]);

  ls.get();

  for (int i = 0; i < a[1]; i++) {
    char row[7];
    ls.getline(row, 7);
    for (int j = 0; j < a[0]; j++) {
      if (row[j] == '\0')
	break;
      else {
	std::array<tile*, 3> t = CHARS[row[j]];
	if (t[0] != NULL)
	  b->add(t[0], i, j);
	if (t[1] != NULL)
	  f->add(t[1], i, j);
	if (t[2] != NULL)
	  r->add(t[2], i, j);
      }
    }
  }
}

#endif
