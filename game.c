#include "game.h"

game_t *create_game(int pixel_size)
{
	game_t *game = (game_t*)malloc(sizeof(game_t));
	if (!game)
		return NULL;

	game->player = create_player(0, 0, 16, 18);
	if (!game->player)
		return NULL;

	game->pixel_size = pixel_size;
	game->state = playing;

	return game;
}
