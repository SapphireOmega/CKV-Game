#ifndef GAME_H_
#define GAME_H_

#include "player.h"
#include "tile.h"

typedef enum {
	playing,
	quit,
} GameState;

typedef struct {
	int x;
	int y;
	int w;
	int h;
} GameWindow;

typedef struct {
	Player *player;
	TileVec tiles;
	int pixel_size;
	float scale;
	SDL_DisplayMode *display_mode;
	GameWindow window;
	GameState state;
} Game;

Game *create_game(SDL_DisplayMode *display_mode, int pixel_size, float scale);
int load_level(const char *file_name, Game* game);

#endif // GAME_H_
