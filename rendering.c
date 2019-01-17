#include "rendering.h"

void load_textures(SDL_Renderer* renderer)
{
	SDL_Surface *tile_sheet_surface = IMG_Load("assets/tilesheet.png");
	tile_sheet = SDL_CreateTextureFromSurface(renderer, tile_sheet_surface);
	SDL_FreeSurface(tile_sheet_surface);
}

void render_player(SDL_Renderer *renderer, const GameWindow *game_window, const Player *player, int pixel_size)
{
	SDL_Rect src;
	src.x = 49;
	src.y = 0;
	src.w = player->rect.w;
	src.h = player->rect.h;

	SDL_Rect dst;
	dst.x = player->rect.x * pixel_size + game_window->x;
	dst.y = player->rect.y * pixel_size + game_window->y;
	dst.w = player->rect.w * pixel_size;
	dst.h = player->rect.h * pixel_size;

	if (player->flip) {
		SDL_RenderCopyEx(renderer, tile_sheet, &src, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
	} else {
		SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
	}
}

void render_tiles(SDL_Renderer *renderer, const GameWindow *game_window, const TileVec *tiles, int pixel_size)
{
	for (int i = 0; i < tiles->used; i++) {
		SDL_Rect src;
		src.w = tiles->vec[i]->rect.w;
		src.h = tiles->vec[i]->rect.h;
		SDL_Rect dst;
		dst.x = tiles->vec[i]->rect.x * pixel_size + game_window->x;
		dst.y = tiles->vec[i]->rect.y * pixel_size + game_window->y;
		dst.w = tiles->vec[i]->rect.w * pixel_size;
		dst.h = tiles->vec[i]->rect.h * pixel_size;
		switch (tiles->vec[i]->type) {
		case grass:
			src.x = 0;
			src.y = 0;
			SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
			break;
		case grassy_dirt:
			src.x = 16;
			src.y = 16;
			SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
			break;
		case dirt:
			src.x = 16;
			src.y = 32;
			SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
			break;
		case dirt_bottom:
			src.x = 16;
			src.y = 48;
			SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
			break;
		default:
			SDL_SetRenderDrawColor(renderer, 255, 0, 200, 255);
			SDL_RenderFillRect(renderer, &dst);
			break;
		}
	}
}

void render_bars(SDL_Renderer *renderer, const GameWindow *game_window, SDL_DisplayMode *display_mode)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect bar;
	bar.x = 0;
	bar.y = 0;
	bar.w = game_window->x;
	bar.h = display_mode->h;
	SDL_RenderFillRect(renderer, &bar);
	bar.x = game_window->x + game_window->w;
	SDL_RenderFillRect(renderer, &bar);
	bar.x = game_window->x;
	bar.w = game_window->w;
	bar.h = game_window->y;
	SDL_RenderFillRect(renderer, &bar);
	bar.y = game_window->y + game_window->h;
	SDL_RenderFillRect(renderer, &bar);
}

void render_playing_state(SDL_Renderer *renderer, const Game *game)
{
	render_player(renderer, &game->window, game->player, game->pixel_size);
	if (game->tiles.vec != NULL)
		render_tiles(renderer, &game->window, &game->tiles, game->pixel_size);
	
	render_bars(renderer, &game->window, game->display_mode);
}

void render_game(SDL_Renderer *renderer, const Game *game)
{
	switch (game->state) {
	case playing:
		render_playing_state(renderer, game);
		break;
	}
}
