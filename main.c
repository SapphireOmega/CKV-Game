#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "game.h"
#include "updating.h"
#include "rendering.h"

#define PIXELSIZE 4

/*
--- TODO ---
* Camera
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

	float scale;
	if ((float)display_mode.w / (float)display_mode.h < 1920.0f / 1080.0f)
		scale = (float)display_mode.w / 1920.0f;
	else
		scale = (float)display_mode.h / 1080.0f;

	int pixel_size = floor(PIXELSIZE * scale);
	scale = (float)pixel_size / (float)PIXELSIZE;

	const Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, flags);
	if (renderer == NULL) {
		fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	load_textures(renderer);

	Game *game = create_game(&display_mode, pixel_size, scale);
	if (!game) {
		fprintf(stderr, "Error creating game\n");
		return EXIT_FAILURE;
	}

	if (init_tile_vec(&game->tiles, 64) != 0) {
		fprintf(stderr, "Error initializing tile_vec\n");
		return EXIT_FAILURE;
	}

	if (push_tile_vec(&game->tiles, create_tile(dirt, 16, 16, 16, 16)) != 0) {
		fprintf(stderr, "Error creating tile\n");
		return EXIT_FAILURE;
	}

	if (load_level("levels/first.lvl", game) != 0) {
		fprintf(stderr, "Error loading level\n");
		return EXIT_FAILURE;
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
		SDL_RenderPresent(renderer);
	}

	free_tile_vec(&game->tiles);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
