#include "tile.h"

tile_t *create_tile(tiletype t, int x, int y, int w, int h)
{
	tile_t *tile = (tile_t*)malloc(sizeof(tile_t));
	if (!tile)
		return NULL;
	tile->rect.x = x;
	tile->rect.y = y;
	tile->rect.w = w;
	tile->rect.h = h;
	tile->type = t;
	return tile;
}

int init_tile_vec(tile_vec_t *tile_vec, size_t init_size)
{
	tile_vec->vec = (tile_t**)malloc(init_size*sizeof(tile_t*));
	if (!tile_vec)
		return 1;
	tile_vec->used = 0;
	tile_vec->size = init_size;
	return 0;
}

int push_tile_vec(tile_vec_t *tile_vec, tile_t *tile)
{
	if (tile_vec->used == tile_vec->size) {
		tile_vec->size *= 2;
		tile_vec->vec = (tile_t**)realloc(tile_vec->vec,
										  tile_vec->size*sizeof(tile_t*));
		if (!tile_vec->vec)
			return 1;
	}
	tile_vec->vec[tile_vec->used++] = tile;
	return 0;
}

void free_tile_vec(tile_vec_t *tile_vec)
{
	for (int i = 0; i < tile_vec->used; i++)
		free(tile_vec->vec[i]);
	free(tile_vec->vec);
	tile_vec->vec = NULL;
	tile_vec->used = 0;
	tile_vec->size = 0;
}
