#ifndef TILES_HPP
#define TILES_HPP

#include <allegro5/allegro.h>

#include "tilemap.hpp"

tile EMPTY, GROUND, WALL, VINE, BROKEN, PIT, BRIDGE, HOLE, BLOCK,
  BUTTON, METAL, RUBBLE;

void init_tiles() {
  EMPTY  = create_colored_tile(16, 16, 0,   0,   0,   0);
  GROUND = create_colored_tile(16, 16, 150, 150, 150, 255);
  WALL   = create_colored_tile(16, 16, 200, 200, 200, 255);
  VINE   = create_colored_tile(16, 16, 100, 100, 255, 255);
  BROKEN = create_colored_tile(16, 16, 100, 100, 100, 255);
  PIT    = create_colored_tile(16, 16, 50,  50,  50,  255);
  BRIDGE = create_colored_tile(16, 16, 255, 230, 180, 255);
  HOLE   = create_colored_tile(16, 16, 10,  10,  10,  255);
  BLOCK  = create_colored_tile(16, 16, 75,  75,  75,  255);
  BUTTON = create_colored_tile(16, 16, 100, 90,  90,  255);
  METAL  = create_colored_tile(16, 16, 90,  90,  100, 255);
  RUBBLE = create_colored_tile(16, 16, 100, 100, 50,  255);
}

#endif
