#ifndef TILE_H_
#define TILE_H_

#include <SDL2/SDL.h>

typedef enum {
	grass,
	grass_left,
	grass_right,
	grass_block,
	grass_block_center,
	grass_block_left,
	grass_block_right,
	grassy_dirt,
	dirt,
	dirt_bottom,
	grassy_dirt_corner_left,
	grassy_dirt_corner_right,
	dirt_side_left,
	dirt_side_right,
	dirt_corner_left,
	dirt_corner_right,
	grassy_dirt_inside_corner_left,
	grassy_dirt_inside_corner_right,
	level_sign,
	jump_left,
	jump_right,
	block_enemy,
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
//int pop_tile_vec(TileVec *tile_vec);
void empty_tile_vec(TileVec *tile_vec, size_t init_size);
void free_tile_vec(TileVec *tile_vec);

#endif // TILE_H_
