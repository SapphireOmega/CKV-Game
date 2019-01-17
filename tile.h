#ifndef TILE_H_
#define TILE_H_

#include <SDL2/SDL.h>

typedef enum {
	grass,
	grassy_dirt,
	dirt,
	dirt_bottom,
} TileType;

typedef struct {
	SDL_Rect rect;
	TileType type;
	int collidable;
} Tile;

Tile *create_tile(TileType t, int x, int y, int w, int h);

typedef struct {
	Tile **vec;
	size_t used;
	size_t size;
} TileVec;

int init_tile_vec(TileVec *tile_vec, size_t init_size);
int push_tile_vec(TileVec *tile_vec, Tile *tile);
//int pop_tile_vec(tile_vec *tile_vec);
void free_tile_vec(TileVec *tile_vec);

#endif // TILE_H_
