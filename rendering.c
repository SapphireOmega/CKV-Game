#include "rendering.h"

void load_textures(SDL_Renderer* renderer)
{
	SDL_Surface *tile_sheet_surface = IMG_Load("assets/game_tile_set.png");
	tile_sheet = SDL_CreateTextureFromSurface(renderer, tile_sheet_surface);
	SDL_FreeSurface(tile_sheet_surface);
}

void render_player(SDL_Renderer *renderer, const player_t *player, int pixel_size)
{
	SDL_Rect src;
	src.x = 2;
	src.y = 16;
	src.w = player->rect.w;
	src.h = player->rect.h;

	SDL_Rect dst;
	dst.x = player->rect.x * pixel_size;
	dst.y = player->rect.y * pixel_size;
	dst.w = player->rect.w * pixel_size;
	dst.h = player->rect.h * pixel_size;

	if (player->flip) {
		SDL_RenderCopyEx(renderer, tile_sheet, &src, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
	} else {
		SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
	}
}

void render_tiles(SDL_Renderer *renderer, const tile_vec_t *tiles, int pixel_size)
{
	for (int i = 0; i < tiles->used; i++) {
		SDL_Rect src;
		src.w = tiles->vec[i]->rect.w;
		src.h = tiles->vec[i]->rect.h;
		SDL_Rect dst;
		dst.x = tiles->vec[i]->rect.x * pixel_size;
		dst.y = tiles->vec[i]->rect.y * pixel_size;
		dst.w = tiles->vec[i]->rect.w * pixel_size;
		dst.h = tiles->vec[i]->rect.h * pixel_size;
		switch (tiles->vec[i]->type) {
		case dirt:
			src.x = 0;
			src.y = 48;
			SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
			break;
		case grass:
			src.x = 0;
			src.y = 32;
			SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
			break;
		case stone:
			src.x = 0;
			src.y = 64;
			SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
		}
	}
}

void render_playing_state(SDL_Renderer *renderer, const game_t *game)
{
	if (game->tiles.vec != NULL)
		render_tiles(renderer, &game->tiles, game->pixel_size);
	render_player(renderer, game->player, game->pixel_size);
}

void render_game(SDL_Renderer *renderer, const game_t *game)
{
	switch (game->state) {
	case playing:
		render_playing_state(renderer, game);
		break;
	}
}
