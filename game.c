#include "game.h"

Game *create_game(SDL_DisplayMode *display_mode, int pixel_size, float scale)
{
	Game *game = (Game*)malloc(sizeof(Game));
	if (!game)
		return NULL;

	game->player = create_player(0, 0);
	if (!game->player)
		return NULL;

	game->pixel_size = pixel_size;
	game->scale = scale;
	game->display_mode = display_mode;
	game->window.w = scale * 1920;
	game->window.h = scale * 1080;
	game->window.x = (display_mode-> w - game->window.w) / 2;
	game->window.y = (display_mode->h - game->window.h) / 2;
	game->state = playing;

	return game;
}
