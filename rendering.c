#include "rendering.h"

const SDL_Color player_color = { .r = 190, .g = 190, .b = 190 };

void load_textures(SDL_Renderer* renderer)
{
	SDL_Surface *dirt_surface = IMG_Load("assets/dirt.png");
	SDL_Surface *grass_surface = IMG_Load("assets/grass.png");
	SDL_Surface *player_surface = IMG_Load("assets/little_guy.png");
	dirt_texture = SDL_CreateTextureFromSurface(renderer, dirt_surface);
	grass_texture = SDL_CreateTextureFromSurface(renderer, grass_surface);
	player_texture = SDL_CreateTextureFromSurface(renderer, player_surface);
	SDL_FreeSurface(dirt_surface);
	SDL_FreeSurface(grass_surface);
	SDL_FreeSurface(player_surface);
}

void render_player(SDL_Renderer *renderer, const player_t *player, int pixel_size)
{
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = player->render_rect.w;
	src.h = player->render_rect.h;

	SDL_Rect dst;
	dst.x = player->render_rect.x * pixel_size;
	dst.y = player->render_rect.y * pixel_size;
	dst.w = player->render_rect.w * pixel_size;
	dst.h = player->render_rect.h * pixel_size;

	SDL_RenderCopy(renderer, player_texture, &src, &dst);
}

void render_tiles(SDL_Renderer *renderer, const tile_vec_t *tiles, int pixel_size)
{
	for (int i = 0; i < tiles->used; i++) {
		SDL_Rect dst;
		dst.x = tiles->vec[i]->rect.x * pixel_size;
		dst.y = tiles->vec[i]->rect.y * pixel_size;
		dst.w = tiles->vec[i]->rect.w * pixel_size;
		dst.h = tiles->vec[i]->rect.h * pixel_size;
		switch (tiles->vec[i]->type) {
		case dirt:
			SDL_RenderCopy(renderer, dirt_texture, NULL, &dst);
			break;
		case grass:
			SDL_RenderCopy(renderer, grass_texture, NULL, &dst);
			break;
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
