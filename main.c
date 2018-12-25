#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "game.h"
#include "updating.h"
#include "rendering.h"

#define PIXELSIZE 5

/*
--- TODO ---
* Deallocate the game structure, player structure etc...
* Inventory
* UI
*/

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	SDL_Window *window = SDL_CreateWindow("window", 
										  SDL_WINDOWPOS_CENTERED, 
										  SDL_WINDOWPOS_CENTERED,
										  display_mode.w, display_mode.h,
										  SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (window == NULL) {
		fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	float scale = display_mode.w / 2560.0f;
	int pixel_size = PIXELSIZE * scale;
	scale = (float)pixel_size / (float)PIXELSIZE;

	const Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, flags);
	if (renderer == NULL) {
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	load_textures(renderer);

	game_t *game = create_game(&display_mode, pixel_size, scale);
	if (!game) {
		fprintf(stderr, "Error creating game\n");
		return EXIT_FAILURE;
	}

	if (init_tile_vec(&game->tiles, 16) != 0) {
		fprintf(stderr, "Error initializing tile_vec\n");
		return EXIT_FAILURE;
	}
	
	for (int x = 0; x < 35; x++) {
		if (push_tile_vec(&game->tiles, create_tile(grass, x * 16, 130, 16, 16)) != 0) {
			fprintf(stderr, "Error pushing back tile to tile_vec\n");
			return EXIT_FAILURE;
		}

		if (push_tile_vec(&game->tiles, create_tile(dirt, x * 16, 146, 16, 16)) != 0) {
			fprintf(stderr, "Error pushing back tile to tile_vec\n");
			return EXIT_FAILURE;
		}

		if (push_tile_vec(&game->tiles, create_tile(stone, x * 16, 162, 16, 16)) != 0) {
			fprintf(stderr, "Error pushing back tile to tile_vec\n");
			return EXIT_FAILURE;
		}
	}

	Uint32 old_time = 0;
	Uint32 new_time = 0;
	float dt = 0.0;

	while (game->state != quit) {
		old_time = new_time;
		new_time = SDL_GetTicks();
		dt = (new_time - old_time) / 1000.0f;

		update_game(game, dt);

		SDL_SetRenderDrawColor(renderer, 99, 155, 255, 255);
		SDL_RenderClear(renderer);
		render_game(renderer, game);
		// SDL_Rect bar;
		// bar.x = 0;
		// bar.y = 0;
		// bar.w = display_mode.w;
		// bar.h = bar_height;
		// SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		// SDL_RenderFillRect(renderer, &bar);
		// bar.y = display_mode.h - bar.h;
		// SDL_RenderFillRect(renderer, &bar);
		SDL_RenderPresent(renderer);
	}

	free_tile_vec(&game->tiles);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
