#include <stdio.h>

#include "tile.h"

Tile *create_tile(TileType t, int x, int y, int w, int h)
{
	Tile *tile = (Tile*)malloc(sizeof(Tile));
	if (!tile)
		return NULL;

	tile->rect.x = x;
	tile->rect.y = y;
	tile->rect.w = w;
	tile->rect.h = h;
	tile->type = t;

	switch (t) {
	case grass:
		tile->collidable = 0;
		break;
	default:
		tile->collidable = 1;
		break;
	}

	return tile;
}

int init_tile_vec(TileVec *tile_vec, size_t init_size)
{
	tile_vec->vec = (Tile**)malloc(init_size*sizeof(Tile*));
	if (!tile_vec)
		return 1;
	tile_vec->used = 0;
	tile_vec->size = init_size;
	return 0;
}

int push_tile_vec(TileVec *tile_vec, Tile *tile)
{
	printf("size: %i, used: %i\n", tile_vec->size, tile_vec->used);
	if (tile_vec->used == tile_vec->size) {
		tile_vec->size *= 2;
		printf("%i\n", tile_vec->size*sizeof(Tile*));
		tile_vec->vec = (Tile**)realloc(tile_vec->vec,
		                                tile_vec->size*sizeof(Tile*));
		if (!tile_vec->vec)
			return 1;
	}
	tile_vec->vec[tile_vec->used++] = tile;
	return 0;
}

void free_tile_vec(TileVec *tile_vec)
{
	for (int i = 0; i < tile_vec->used; i++)
		free(tile_vec->vec[i]);
	free(tile_vec->vec);
	tile_vec->vec = NULL;
	tile_vec->used = 0;
	tile_vec->size = 0;
}

void empty_tile_vec(TileVec *tile_vec, size_t init_size)
{
	for (int i = 0; i < tile_vec->used; i++)
		free(tile_vec->vec[i]);
	tile_vec->used = 0;
	tile_vec->size = init_size;
	tile_vec->vec = (Tile**)realloc(tile_vec->vec,
									tile_vec->size*sizeof(Tile*));
}
