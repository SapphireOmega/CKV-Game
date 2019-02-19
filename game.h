#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>

#include "player.h"
#include "tile.h"
#include "camera.h"
#include "enemy.h"

const static char *levels[2] = {
	"levels/first.lvl",
	"levels/level2.lvl",
};

typedef enum {
	start,
	playing,
	death,
	quit,
} GameState;

typedef struct {
	int x;
	int y;
	int w;
	int h;
	int pixel_size;
} GameWindow;

typedef struct {
	Player *player;
	TileVec tiles;
	TileVec bg_tiles;
	TileVec inv_tiles;
	EnemyVec enemies;
	float scale;
	SDL_DisplayMode *display_mode;
	GameWindow window;
	GameState state;
	Camera *current_camera;
	Uint32 level;
	Uint32 level_width;
	Uint32 level_height;
} Game;

Game *create_game(SDL_DisplayMode *display_mode, int pixel_size, float scale);
int load_level(const char *file_name, Game *game);
int next_level(Game *game);

void destroy_game(Game *game);

#endif // GAME_H_
