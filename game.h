#ifndef GAME_H_
#define GAME_H_

#include "player.h"
#include "tile.h"

typedef enum {
	playing,
	quit,
} game_state;

typedef struct {
	int x;
	int y;
	int w;
	int h;
} game_window_t;

typedef struct {
	player_t *player;
	tile_vec_t tiles;
	int pixel_size;
	float scale;
	SDL_DisplayMode *display_mode;
	game_window_t window;
	game_state state;
} game_t;

game_t *create_game(SDL_DisplayMode *display_mode, int pixel_size, float scale);

#endif // GAME_H_
