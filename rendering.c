#include "rendering.h"

#define DEATH_FADE_STEP 2

void
load_text(SDL_Renderer *renderer, const GameWindow *game_window)
{
	font = TTF_OpenFont("fonts/pixelart/pixelart.ttf", 20);
	death_font = TTF_OpenFont("fonts/pixelart/pixelart.ttf", 40);
	SDL_Color start_text_color = {255, 255, 255, 255};
	SDL_Color death_text_color = {200, 0, 0, 255};
	start_text_surface = TTF_RenderText_Solid(font, "press space to start", start_text_color);
	death_text_surface = TTF_RenderText_Solid(death_font, "you died", death_text_color);
	start_text = SDL_CreateTextureFromSurface(renderer, start_text_surface);
	death_text = SDL_CreateTextureFromSurface(renderer, death_text_surface);
	SDL_FreeSurface(start_text_surface);
	SDL_FreeSurface(death_text_surface);
	SDL_QueryTexture(start_text, NULL, NULL, &start_text_rect.w, &start_text_rect.h);
	SDL_QueryTexture(death_text, NULL, NULL, &death_text_rect.w, &death_text_rect.h);
	start_text_rect.x = game_window->x + game_window->w / 2 - start_text_rect.w / 2;
	start_text_rect.y = game_window->y + game_window->h / 5 * 2 - start_text_rect.h / 2; 
	death_text_rect.x = game_window->x + game_window->w / 2 - death_text_rect.w / 2;
	death_text_rect.y = game_window->y + game_window->h / 5 * 2 - death_text_rect.h / 2; 
}

void
load_textures(SDL_Renderer *renderer)
{
	SDL_Surface *tile_sheet_surface = IMG_Load("assets/tilesheet.png");
	tile_sheet = SDL_CreateTextureFromSurface(renderer, tile_sheet_surface);
	SDL_FreeSurface(tile_sheet_surface);
}

void
render_player(SDL_Renderer *renderer, const GameWindow *game_window, const Camera *camera, const Player *player)
{
	int pixel_size = game_window->pixel_size;

	int w_growth = 0;

	SDL_Rect src;
	src.x = 96;
	src.y = 32;

	switch (player->state) {
	case player_attack:
		switch (player->attack_frame) {
		case 0:
			src.x = 49;
			src.y = 0;
			w_growth = 0;
			break;
		case 1:
			src.x = 65;
			src.y = 0;
			w_growth = 4;
			break;
		case 2:
			src.x = 97;
			src.y = 0;
			w_growth = 6;
			break;
		}
		break;
	case player_neutral:
		src.x = 49;
		src.y = 0;
		w_growth = 0;
		break;
	}

	src.w = player->entity->rect.w + w_growth;
	src.h = player->entity->rect.h;

	SDL_Rect dst;
	dst.x = player->entity->rect.x * pixel_size + game_window->x - camera->x * pixel_size;
	dst.y = player->entity->rect.y * pixel_size + game_window->y - camera->y * pixel_size;
	dst.w = src.w * pixel_size;
	dst.h = src.h * pixel_size;

	if (player->flip) {
		dst.x -= w_growth * pixel_size;
		SDL_RenderCopyEx(renderer, tile_sheet, &src, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
	} else {
		SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
	}
}

void
render_tiles(SDL_Renderer *renderer, const GameWindow *game_window, const Camera *camera, const TileVec *tiles)
{
	int pixel_size = game_window->pixel_size;
	for (int i = 0; i < tiles->used; i++) {
		if (tiles->vec[i]->rect.x + tiles->vec[i]->rect.w > camera->x &&
			tiles->vec[i]->rect.x < camera->x + camera->w &&
			tiles->vec[i]->rect.y + tiles->vec[i]->rect.h > camera->y &&
			tiles->vec[i]->rect.y < camera->y + camera->h)
		{
			SDL_Rect src;
			src.x = 96;
			src.y = 32;
			src.w = tiles->vec[i]->rect.w;
			src.h = tiles->vec[i]->rect.h;

			SDL_Rect dst;
			dst.x = tiles->vec[i]->rect.x * pixel_size + game_window->x - camera->x * pixel_size;
			dst.y = tiles->vec[i]->rect.y * pixel_size + game_window->y - camera->y * pixel_size;
			dst.w = tiles->vec[i]->rect.w * pixel_size;
			dst.h = tiles->vec[i]->rect.h * pixel_size;

			switch (tiles->vec[i]->type) {
			case grass:
				src.x = 0; src.y = 0;
				break;
			case grassy_dirt:
				src.x = 16; src.y = 16;
				break;
			case dirt:
				src.x = 16; src.y = 32;
				break;
			case dirt_bottom:
				src.x = 16; src.y = 48;
				break;
			case grassy_dirt_corner_left:
				src.x = 0; src.y = 16;
				break;
			case dirt_side_left:
				src.x = 0; src.y = 32;
				break;
			case dirt_corner_left:
				src.x = 0; src.y = 48;
				break;
			case grassy_dirt_inside_corner_left:
				src.x = 80; src.y = 16;
				break;
			case grassy_dirt_corner_right:
				src.x = 32; src.y = 16;
				break;
			case dirt_side_right:
				src.x = 32; src.y = 32;
				break;
			case dirt_corner_right:
				src.x = 32; src.y = 48;
				break;
			case grassy_dirt_inside_corner_right:
				src.x = 64; src.y = 16;
				break;
			case grass_left:
				src.x = 32; src.y = 0;
				break;
			case grass_right:
				src.x = 16; src.y = 0;
				break;
			case grass_block:
				src.x = 48; src.y = 64;
				break;
			case grass_block_center:
				src.x = 16; src.y = 64;
				break;
			case grass_block_left:
				src.x = 0; src.y = 64;
				break;
			case grass_block_right:
				src.x = 32; src.y = 64;
				break;
			}
			SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
		}
	}
}

void
render_enemies(SDL_Renderer *renderer, const GameWindow *game_window, const Camera* camera, const EnemyVec *enemies)
{
	int pixel_size = game_window->pixel_size;

	for (int i = 0; i < enemies->used; i++) {
		if (enemies->vec[i]->entity->alive &&
			enemies->vec[i]->entity->rect.x + enemies->vec[i]->entity->rect.w > camera->x &&
			enemies->vec[i]->entity->rect.x < camera->x + camera->w &&
			enemies->vec[i]->entity->rect.y + enemies->vec[i]->entity->rect.h > camera->y &&
			enemies->vec[i]->entity->rect.y < camera->y + camera->h)
		{
			SDL_Rect src;
			src.x = 96;
			src.y = 32;
			src.w = enemies->vec[i]->entity->rect.w;
			src.h = enemies->vec[i]->entity->rect.h;

			switch (enemies->vec[i]->type) {
			case dark_magician:
				switch (enemies->vec[i]->state) {
				case enemy_neutral:
					if (enemies->vec[i]->entity->flip) {
						src.x = 98;
						src.y = 64;
					} else {
						src.x = 98;
						src.y = 48;
					}
					break;
				}
				break;
			}

			SDL_Rect dst;
			dst.x = enemies->vec[i]->entity->rect.x * pixel_size + game_window->x - camera->x * pixel_size;
			dst.y = enemies->vec[i]->entity->rect.y * pixel_size + game_window->y - camera->y * pixel_size;
			dst.w = src.w * pixel_size;
			dst.h = src.h * pixel_size;

			SDL_RenderCopy(renderer, tile_sheet, &src, &dst);
		}
	}
}

void
render_bars(SDL_Renderer *renderer, const GameWindow *game_window, SDL_DisplayMode *display_mode)
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

void
render_playing_state(SDL_Renderer *renderer, const Game *game)
{
	render_player(renderer, &game->window, game->current_camera, game->player);

	if (game->enemies.vec != NULL)
		render_enemies(renderer, &game->window, game->current_camera, &game->enemies);

	if (game->tiles.vec != NULL)
		render_tiles(renderer, &game->window, game->current_camera, &game->tiles);

	render_bars(renderer, &game->window, game->display_mode);
}

void
render_start_state(SDL_Renderer *renderer, const Game *game)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect tmp;
	tmp.x = game->window.x;
	tmp.y = game->window.y;
	tmp.w = game->window.w;
	tmp.h = game->window.h;
	SDL_RenderFillRect(renderer, &tmp);
	SDL_RenderCopy(renderer, start_text, NULL, &start_text_rect);
	render_bars(renderer, &game->window, game->display_mode);
}

void
render_death_state(SDL_Renderer *renderer, const Game *game)
{
	render_playing_state(renderer, game);
	int a = DEATH_FADE_STEP * game->player->death_timer;
	if (a >= 255)
		a = 255;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, a);
	SDL_Rect tmp;
	tmp.x = game->window.x;
	tmp.y = game->window.y;
	tmp.w = game->window.w;
	tmp.h = game->window.h;
	SDL_RenderFillRect(renderer, &tmp);
	SDL_SetTextureAlphaMod(death_text, a);
	SDL_RenderCopy(renderer, death_text, NULL, &death_text_rect);
}

void
render_game(SDL_Renderer *renderer, const Game *game)
{
	switch (game->state) {
	case playing:
		render_playing_state(renderer, game);
		break;
	case start:
		render_start_state(renderer, game);
		break;
	case death:
		render_death_state(renderer, game);
		break;
	}
}

void
render_clean()
{
	SDL_DestroyTexture(tile_sheet);
	SDL_DestroyTexture(start_text);
}
