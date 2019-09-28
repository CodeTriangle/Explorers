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
    char s[3];
    ls.getline(s, 2, ',');
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

  l.back   = create_tilemap(l.width, l.height+1, 16, &EMPTY);
  l.fore   = create_tilemap(l.width, l.height+1, 16, &EMPTY);
  l.rubble = create_tilemap(l.width, l.height+1, 16, &EMPTY);

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
	  b = &GROUND;
	  break;
	case 'W':
	  b = &WALL;
	  break;
	case 'V':
	  b = &WALL;
	  r = &VINE;
	  break;
	case 'X':
	  b = &BROKEN;
	  break;
	case 'O':
	  b = &PIT;
	  break;
	case 'B':
	  f = &BLOCK;
	  b = &GROUND;
	  break;
	case '_':
	  b = &GROUND;
	  break;
	case '+':
	  b = &METAL;
	  break;
	case 'R':
	  b = &GROUND;
	  r = &RUBBLE;
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

  return l;
}

#endif
