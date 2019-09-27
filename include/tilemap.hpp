#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>

#include <allegro5/allegro.h>

typedef struct {
  ALLEGRO_BITMAP* bitmap;
  float tx, ty;
} tile;

typedef struct {
  int width, height;
  int tile_size;
  std::vector<std::vector<tile*>> tiles;
  tile *empty;
} tilemap;

tilemap create_tilemap(int w, int h, int ts, tile *e) {
  tilemap tm;
  
  tm.width = w;
  tm.height = h;
  tm.tile_size = ts;
  tm.empty = e;

  for (int i = 0; i < h; i++) {
    std::vector<tile*> v;
    for(int j = 0; j < w; j++) {
      v.push_back(e);
    }
    tm.tiles.push_back(v);
  }
  
  return tm;
}

bool add_tile_to_tilemap(tile *t, tilemap *tm, int r = -1, int c = -1) {
  if (r == -1 && c == -1) {
    for (int row = 0; row < tm->tiles.size(); row++)
      for (int column = 0; column < tm->tiles[row].size(); column++)
	if (tm->tiles[row][column] == tm->empty) {
	  tm->tiles[row][column] = t;
	  return true;
	}
    return false;
  }
  else if (r == -1 || c == -1)
    return false;
  else
    tm->tiles[r][c] = t;
  return true;
}

void draw_tilemap(tilemap *tm) {
  for (int row = 0; row < tm->tiles.size(); row++)
    for (int column = 0; column < tm->tiles[row].size(); column++)
      al_draw_bitmap_region(tm->tiles[row][column]->bitmap,
			    tm->tile_size * tm->tiles[row][column]->tx,
			    tm->tile_size * tm->tiles[row][column]->ty,
			    TILE_SIZE,
			    TILE_SIZE,
			    tm->tile_size * column,
			    tm->tile_size * row,
			    0);
}

#endif
