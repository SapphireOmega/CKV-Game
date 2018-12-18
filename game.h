#ifndef GAME_H_
#define GAME_H_

#include "player.h"
#include "tile.h"

typedef enum {
	playing,
	quit,
} game_state;

typedef struct {
	player_t *player;
	tile_vec_t tiles;
	int pixel_size;
	game_state state;
} game_t;

game_t *create_game(int pixel_size);

#endif // GAME_H_
