#ifndef TILE_H_
#define TILE_H_

#include <SDL2/SDL.h>

typedef enum {
	grass,
	grassy_dirt,
	dirt,
} tiletype;

typedef struct {
	SDL_Rect rect;
	tiletype type;
	int collidable;
} tile_t;

tile_t *create_tile(tiletype t, int x, int y, int w, int h);

typedef struct {
	tile_t **vec;
	size_t used;
	size_t size;
} tile_vec_t;

int init_tile_vec(tile_vec_t *tile_vec, size_t init_size);
int push_tile_vec(tile_vec_t *tile_vec, tile_t *tile);
//int pop_tile_vec(tile_vec_t *tile_vec);
void free_tile_vec(tile_vec_t *tile_vec);

#endif // TILE_H_
