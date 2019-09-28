#ifndef LEVELS_HPP
#define LEVELS_HPP

#include <array>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "tilemap.hpp"
#include "tiles.hpp"

typedef struct {
  int width, height;
  int enter, exit;
  tilemap back, fore, rubble;
  char test;
} level;

level read_level(const char *fn) {
  level l;

  std::ifstream ls(fn);
  
  for (int i = 0; i < 4; i++) {
    char s[4];
    ls.getline(s, 3, ',');
    switch (i) {
    case 0:
      l.width = atoi(s);
      break;
    case 1:
      l.height = atoi(s);
      break;
    case 2:
      l.enter = atoi(s);
      break;
    case 3:
      l.exit = atoi(s);
      break;
    }
  }

  l.back   = create_tilemap(l.width, l.height+1, 16, &MATERIALS["EMPTY"]);
  l.fore   = create_tilemap(l.width, l.height+1, 16, &MATERIALS["EMPTY"]);
  l.rubble = create_tilemap(l.width, l.height+1, 16, &MATERIALS["EMPTY"]);

  ls.get();

  for (int row = 0; row < l.height; row++) {
    int currow = 0, curcol = 0;
    bool loop = true;
    while (loop) {
      char c;
      ls.get(c);
      if (c == '\n') {
	curcol = 0;
	currow++;
      }
      else {
	std::array<tile*, 3> t = CHARS[c];
	if (t[0] != NULL)
	  add_tile_to_tilemap(t[0], &l.back, currow, curcol);
	if (t[1] != NULL)
	  add_tile_to_tilemap(t[1], &l.fore, currow, curcol);
	if (t[2] != NULL)
	  add_tile_to_tilemap(t[2], &l.rubble, currow, curcol);
	
	curcol++;
      }
      loop = ls.good();
    }
  }

  ls.close();

  return l;
}

#endif
