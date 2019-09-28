#ifndef LEVELS_HPP
#define LEVELS_HPP

#include <fstream>
#include <iostream>
#include <cstdlib>

#include "tilemap.hpp"
#include "tiles.hpp"

typedef struct {
  int width, height;
  int enter, exit;
  tilemap back, fore, rubble;
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
    while (ls.good()) {
      char c;
      ls.get(c);
      if (c == '\n') {
	curcol = 0;
	currow++;
      }
      else {
	tile *b = NULL, *f = NULL, *r = NULL;
	switch (c) {
	case '-':
	  b = &MATERIALS["GROUND"];
	  break;
	case 'W':
	  b = &MATERIALS["WALL"];
	  break;
	case 'V':
	  b = &MATERIALS["WALL"];
	  r = &MATERIALS["VINE"];
	  break;
	case 'X':
	  b = &MATERIALS["BROKEN"];
	  break;
	case 'O':
	  b = &MATERIALS["PIT"];
	  break;
	case '=':
	  b = &MATERIALS["BRIDGE"];
	  break;
	case 'B':
	  f = &MATERIALS["BLOCK"];
	  b = &MATERIALS["GROUND"];
	  break;
	case '_':
	  b = &MATERIALS["GROUND"];
	  f = &MATERIALS["BUTTON"];
	  break;
	case '+':
	  b = &MATERIALS["METAL"];
	  break;
	case 'R':
	  b = &MATERIALS["GROUND"];
	  r = &MATERIALS["RUBBLE"];
	  break;
	}
	if (b != NULL)
	  add_tile_to_tilemap(b, &l.back, currow, curcol);
	if (f != NULL)
	  add_tile_to_tilemap(f, &l.fore, currow, curcol);
	if (r != NULL)
	  add_tile_to_tilemap(r, &l.rubble, currow, curcol);
	
	curcol++;
      }
    }
  }

  ls.close();

  return l;
}

#endif
