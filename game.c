#include <stdio.h>
#include <string.h>

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

int load_level(const char *level_name, Game *game)
{
	empty_tile_vec(&game->tiles, 128);

	FILE *file = fopen(level_name, "r");
	char buff[256];
	int y = 0;

	while (fgets(buff, sizeof(buff), file)) {
		buff[strcspn(buff, "\n")] = '\0';
		for (int x = 0; x < strlen(buff); x++) {
			switch (buff[x]) {
			case '0':
				break;
			case '1':
				push_tile_vec(&game->tiles, create_tile(grassy_dirt, x * 16, y * 16, 16, 16));
				push_tile_vec(&game->tiles, create_tile(grass, x * 16, (y - 1) * 16, 16, 16));
				break;
			case '2':
				push_tile_vec(&game->tiles, create_tile(dirt, x * 16, y * 16, 16, 16));
				break;
			case '3':
				push_tile_vec(&game->tiles, create_tile(dirt_bottom, x * 16, y * 16, 16, 16));
				break;
			default:
				fprintf(stderr, "Undefined tiletype %c in level %s\n", buff[x], level_name);
				return EXIT_FAILURE;
				break;
			}
		}
		y++;
	}

	return fclose(file);
}
