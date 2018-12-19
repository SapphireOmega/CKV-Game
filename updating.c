#include "updating.h"

#define GRAV 2000
#define SPEED 100
#define JUMP 350

void update_player(player_t *player, tile_vec_t *tile_vec, float dt)
{
	player->vel_y += GRAV * dt;

	player->pos_x += player->vel_x * dt;
	player->collider.x = (int)player->pos_x;
	player->render_rect.x = player->collider.x - player->x_render_offset;
	for (int i = 0; i < tile_vec->used; i++) {
		tile_t tile = *tile_vec->vec[i];
		if (player->collider.x + player->collider.w > tile.rect.x &&
			player->collider.y < tile.rect.y + tile.rect.h &&
			player->collider.y + player->collider.h > tile.rect.y &&
			player->collider.x < tile.rect.x)
		{
			player->pos_x = tile.rect.x - player->collider.w;
			player->collider.x = (int)player->pos_x;
			player->render_rect.x = player->collider.x - player->x_render_offset;
		}
		else if (player->collider.x < tile.rect.x + tile.rect.w &&
			player->collider.y < tile.rect.y + tile.rect.h &&
			player->collider.y + player->collider.h > tile.rect.y &&
			player->collider.x + player->collider.w > tile.rect.x + tile.rect.w)
		{
			player->pos_x = tile.rect.x + tile.rect.w;
			player->collider.x = (int)player->pos_x;
			player->render_rect.x = player->collider.x - player->x_render_offset;
		}
	}

	player->onground = 0;
	player->pos_y += player->vel_y * dt;
	player->collider.y = (int)player->pos_y;
	player->render_rect.y = player->collider.y - player->x_render_offset;
	for (int i = 0; i < tile_vec->used; i++) {
		tile_t tile = *tile_vec->vec[i];
		if (player->collider.y + player->collider.h >= tile.rect.y &&
			player->collider.x + player->collider.w > tile.rect.x &&
			player->collider.x < tile.rect.x + tile.rect.w &&
			player->collider.y < tile.rect.y)
		{
			player->vel_y = 0.0f;
			player->pos_y = tile.rect.y - player->collider.h;
			player->collider.y = (int)player->pos_y;
			player->render_rect.y = player->collider.y - player->x_render_offset;
			player->onground = 1;
		}
		else if (player->collider.y < tile.rect.y + tile.rect.h &&
			player->collider.x + player->collider.w > tile.rect.x &&
			player->collider.x < tile.rect.x + tile.rect.w &&
			player->collider.y + player->collider.h > tile.rect.y + tile.rect.h)
		{
			player->vel_y = 0.0f;
			player->pos_y = tile.rect.y + tile.rect.h;
			player->collider.y = (int)player->pos_y;
			player->render_rect.y = player->collider.y - player->x_render_offset;
		}
	}
}

void update_playing_state(game_t *game, float dt)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			game->state = quit;
			break;
		case SDL_KEYDOWN:
			if (!event.key.repeat) switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				game->state = quit;
				break;
			case SDLK_w:
				if (game->player->onground)
					game->player->vel_y -= JUMP;
				break;
			case SDLK_a:
				if (!game->player->left) {
					game->player->left = 1;
					game->player->vel_x -= SPEED;
				}
				break;
			case SDLK_d:
				if (!game->player->right) {
					game->player->right = 1;
					game->player->vel_x += SPEED;
				}
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_a:
				game->player->left = 0;
				game->player->vel_x += SPEED;
				break;
			case SDLK_d:
				game->player->right = 0;
				game->player->vel_x -= SPEED;
				break;
			}
		}
	}
	update_player(game->player, &game->tiles,  dt);
}

void update_game(game_t *game, float dt)
{
	switch (game->state) {
	case playing:
		update_playing_state(game, dt);
		break;
	}
}
