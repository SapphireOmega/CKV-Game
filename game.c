#include <stdio.h>
#include <string.h>

#include "game.h"

Game *
create_game(SDL_DisplayMode *display_mode, int pixel_size, float scale)
{
	Game *game = (Game*)malloc(sizeof(Game));
	if (!game)
		return NULL;

	game->scale = scale;
	game->display_mode = display_mode;
	game->window.w = scale * 1920;
	game->window.h = scale * 1080;
	game->window.x = (display_mode-> w - game->window.w) / 2;
	game->window.y = (display_mode->h - game->window.h) / 2;
	game->window.pixel_size = pixel_size;
	game->state = start;
	game->level = 0;

	int cam_w = game->window.w / pixel_size;
	int cam_h = game->window.h / pixel_size;
	Camera *player_camera = create_camera(0, 0, cam_w, cam_h);
	game->player = create_player(0, 0, player_camera);
	if (!game->player)
		return NULL;

	game->current_camera = player_camera;

	return game;
}

int
load_level(const char *level_name, Game *game)
{
	respawn_entity(game->player->entity);
	empty_tile_vec(&game->tiles, 128);
	empty_tile_vec(&game->bg_tiles, 128);
	empty_tile_vec(&game->inv_tiles, 128);
	empty_enemy_vec(&game->enemies, 32);

	FILE *file = fopen(level_name, "r");
	char buff[256];
	int y = 0;
	unsigned int level_width = 0;
	unsigned int level_height = 0;

	while (fgets(buff, sizeof(buff), file)) {
		buff[strcspn(buff, "\n")] = '\0';
		unsigned int new_level_width = strlen(buff);
		level_height++;
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
			case 'b':
				push_tile_vec(&game->tiles, create_tile(grassy_dirt_inside_corner_right, x * 16, y * 16, 16, 16));
				push_tile_vec(&game->tiles, create_tile(grass_right, x * 16, (y - 1) * 16, 16, 16));
				break;
			case 'c':
				push_tile_vec(&game->tiles, create_tile(grass_block, x * 16, y * 16, 16, 16));
				push_tile_vec(&game->tiles, create_tile(grass, x * 16, (y - 1) * 16, 16, 16));
				break;
			case 'd':
				push_tile_vec(&game->tiles, create_tile(grass_block_right, x * 16, y * 16, 16, 16));
				push_tile_vec(&game->tiles, create_tile(grass, x * 16, (y - 1) * 16, 16, 16));
				break;
			case 'e':
				push_tile_vec(&game->tiles, create_tile(grass_block_center, x * 16, y * 16, 16, 16));
				push_tile_vec(&game->tiles, create_tile(grass, x * 16, (y - 1) * 16, 16, 16));
				break;
			case 'f':
				push_tile_vec(&game->tiles, create_tile(grass_block_left, x * 16, y * 16, 16, 16));
				push_tile_vec(&game->tiles, create_tile(grass, x * 16, (y - 1) * 16, 16, 16));
				break;
			case 'g':
				push_enemy_vec(&game->enemies, create_enemy(x * 16, y * 16, dark_magician));
				break;
			case 'h':
				push_tile_vec(&game->bg_tiles, create_tile(level_sign, x * 16, y * 16, 16, 16));
				break;
			case 'i':
				push_tile_vec(&game->inv_tiles, create_tile(jump_right, x * 16, y * 16, 16, 16));
				break;
			case 'j':
				push_tile_vec(&game->inv_tiles, create_tile(jump_left, x * 16, y * 16, 16, 16));
				break;
			case 'k':
				push_tile_vec(&game->inv_tiles, create_tile(block_enemy, x * 16, y * 16, 16, 16));
				break;
			default:
				fprintf(stderr, "Undefined tiletype %c in level %s\n", buff[x], level_name);
				break;
			}
		}
		y++;
	}

	game->level_width = level_width;
	game->level_height = level_height;
	return fclose(file);
}

int
next_level(Game *game)
{
	if (++game->level > 1)
		game->level = 0;

	return load_level(levels[game->level], game);
}

void
destroy_game(Game *game)
{
	destroy_player(game->player);
	free(game);
}
