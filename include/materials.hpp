#ifndef TILES_HPP
#define TILES_HPP

#include <vector>
#include <fstream>
#include <map>
#include <array>
#include <string>
#include <cstdlib>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "tilemap.hpp"

std::map<std::string, tile> MATERIALS;
std::map<char, std::array<tile*, 3>> CHARS;
std::map<std::string, SDL_Surface*> SURFACES;
std::map<std::string, SDL_Texture*> TEXTURES;
std::vector<tile> TEXTS;

void load_image(SDL_Renderer* renderer, const char* fn) {
  if (SURFACES.find(fn) == SURFACES.end()) {
    int x, y, n;
    unsigned char* data = stbi_load(fn, &x, &y, &n, STBI_rgb_alpha);

    if (data == NULL) {
      SDL_Log("Loading image failed: %s\n", stbi_failure_reason());
      exit(1);
    }

    SDL_Surface* s = SDL_CreateRGBSurfaceWithFormatFrom((void*) data, x, y,
							32, 4 * x,
							SDL_PIXELFORMAT_RGBA32);
	
    SURFACES.insert(std::make_pair(std::string(fn), s));
    TEXTURES.insert(std::make_pair(std::string(fn), SDL_CreateTextureFromSurface(renderer, s)));
  }  
}

void init_materials(SDL_Renderer* renderer) {
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

      tile t(renderer, a[0], a[1], a[2], a[3], TILE_SIZE, TILE_SIZE);
      
      MATERIALS.insert(std::make_pair(std::string(name), t));
    }
    
    else if(std::string(func) == "br") {
      std::string rn;
      unsigned char a[2];

      for (int i = 0; i < 3; i++) {
	char c[20];
	ms.getline(c, 20, ',');
	if (i == 0)
	  rn = std::string(c);
	else
	  a[i-1] = (unsigned char) atoi(c);
      }

      rn = "assets/" + rn;

      load_image(renderer, rn.c_str());

      tile t(TEXTURES[rn], a[0], a[1], 16);

      MATERIALS.insert(std::make_pair(std::string(name), t));
    }
    
    else if(std::string(func) == "ss") {
      std::string rn;
      char c[10];
      
      ms.getline(c, 10, ',');
      rn = std::string(c);
      rn = "assets/" + rn;

      load_image(renderer, rn.c_str());

      for (int i = 0; i < 5; i++) {
	char d[11];
	sprintf(d, "%s%d", name, i);

	tile t(TEXTURES[rn], i%2 == 1 ? 1 : 0, i<2 ? 0 : 1, 16);
	MATERIALS.insert(std::make_pair(std::string(d), t));
      }
    }
    ms.get();
  }
  ms.close();

  load_image(renderer, "assets/texts.png");

  for (int i = 0; i < 5; i++) {
    tile t(TEXTURES["assets/texts.png"], 0, i, 80, 8);
    TEXTS.push_back(t);
  }
  
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

bool is_collidable(tile* t) {
  if (t == &MATERIALS["WALL"] ||
      t == &MATERIALS["RUBBLE"] ||
      t == &MATERIALS["RUB2"] ||
      t == &MATERIALS["RUB3"] ||
      t == &MATERIALS["SWITCH"])
    return true;
  return false;
}

bool is_player(tile *t) {
  if (t == &MATERIALS["PLAYER0"] ||
      t == &MATERIALS["PLAYER1"] ||
      t == &MATERIALS["PLAYER2"] ||
      t == &MATERIALS["PLAYER3"] ||
      t == &MATERIALS["FPLAY0"] ||
      t == &MATERIALS["FPLAY1"] ||
      t == &MATERIALS["FPLAY2"] ||
      t == &MATERIALS["FPLAY3"])
    return true;
  return false;
}

bool is_rubble(tile *t) {
  if (t == &MATERIALS["RUBBLE"] ||
      t == &MATERIALS["RUB2"] ||
      t == &MATERIALS["RUB3"])
    return true;
  return false;
}
    

tile* player(int d, bool f) {
  char c[8];
  if (f)
    sprintf(c, "FPLAY%d", d);
  else
    sprintf(c, "PLAYER%d", d);
  return &MATERIALS[c];
}

int flip(int d) {
  return (d + 2) % 4;
}

#endif
