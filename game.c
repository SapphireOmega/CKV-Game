#include <stdio.h>
#include <string.h>

#include "game.h"

Game *create_game(SDL_DisplayMode *display_mode, int pixel_size, float scale)
{
	Game *game = (Game*)malloc(sizeof(Game));
	if (!game)
		return NULL;

	game->pixel_size = pixel_size;
	game->scale = scale;
	game->display_mode = display_mode;
	game->window.w = scale * 1920;
	game->window.h = scale * 1080;
	game->window.x = (display_mode-> w - game->window.w) / 2;
	game->window.y = (display_mode->h - game->window.h) / 2;
	game->state = start;

	int cam_w = game->window.w / pixel_size;
	int cam_h = game->window.h / pixel_size;
	Camera *player_camera = create_camera(0, 0, cam_w, cam_h);
	game->player = create_player(0, 0, player_camera);
	if (!game->player)
		return NULL;

	game->current_camera = player_camera;

	return game;
}

int load_level(const char *level_name, Game *game)
{
	empty_tile_vec(&game->tiles, 128);

	FILE *file = fopen(level_name, "r");
	char buff[256];
	int y = 0;
	unsigned int level_width = 0;

	while (fgets(buff, sizeof(buff), file)) {
		buff[strcspn(buff, "\n")] = '\0';
		unsigned int new_level_width = strlen(buff);
		if (new_level_width > level_width) 
			level_width = new_level_width;
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
			case '4':
				push_tile_vec(&game->tiles, create_tile(dirt_side_left, x * 16, y * 16, 16, 16));
				break;
			case '5':
				push_tile_vec(&game->tiles, create_tile(grassy_dirt_corner_left, x * 16, y * 16, 16, 16));
				push_tile_vec(&game->tiles, create_tile(grass, x * 16, (y - 1) * 16, 16, 16));
				break;
			case '6':
				push_tile_vec(&game->tiles, create_tile(dirt_corner_left, x * 16, y * 16, 16, 16));
				break;
			case '7':
				push_tile_vec(&game->tiles, create_tile(grassy_dirt_inside_corner_left, x * 16, y * 16, 16, 16));
				push_tile_vec(&game->tiles, create_tile(grass_left, x * 16, (y - 1) * 16, 16, 16));
				break;
			case '8':
				push_tile_vec(&game->tiles, create_tile(dirt_side_right, x * 16, y * 16, 16, 16));
				break;
			case '9':
				push_tile_vec(&game->tiles, create_tile(grassy_dirt_corner_right, x * 16, y * 16, 16, 16));
				push_tile_vec(&game->tiles, create_tile(grass, x * 16, (y - 1) * 16, 16, 16));
				break;
			case 'a':
				push_tile_vec(&game->tiles, create_tile(dirt_corner_right, x * 16, y * 16, 16, 16));
				break;
			default:
				fprintf(stderr, "Undefined tiletype %c in level %s\n", buff[x], level_name);
				return EXIT_FAILURE;
				break;
			}
		}
		y++;
	}

	game->level_width = level_width;
	return fclose(file);
}
