#include "updating.h"

#define GRAV 2000
#define SPEED 100
#define JUMP 350
#define CAMERA_MARGIN_X 160
#define CAMERA_MARGIN_Y 63

void update_player(Player *player, TileVec *tile_vec, unsigned int level_width, float dt)
{
	player->vel_y += GRAV * dt;

	player->pos_x += player->vel_x * dt;
	player->rect.x = (int)player->pos_x;
	for (int i = 0; i < tile_vec->used; i++) {
		Tile tile = *tile_vec->vec[i];
		if (tile.collidable) {
			if (player->rect.x + player->rect.w > tile.rect.x &&
				player->rect.y < tile.rect.y + tile.rect.h &&
				player->rect.y + player->rect.h > tile.rect.y &&
				player->rect.x < tile.rect.x)
			{
				player->pos_x = tile.rect.x - player->rect.w;
				player->rect.x = (int)player->pos_x;
			}
			else if (player->rect.x < tile.rect.x + tile.rect.w &&
				player->rect.y < tile.rect.y + tile.rect.h &&
				player->rect.y + player->rect.h > tile.rect.y &&
				player->rect.x + player->rect.w > tile.rect.x + tile.rect.w)
			{
				player->pos_x = tile.rect.x + tile.rect.w;
				player->rect.x = (int)player->pos_x;
			}
		}
	}

	if (player->rect.x + player->rect.w + CAMERA_MARGIN_X > player->camera->x + player->camera->w)
		player->camera->x = player->rect.x + player->rect.w + CAMERA_MARGIN_X - player->camera->w;
	else if (player->rect.x - CAMERA_MARGIN_X < player->camera->x)
		player->camera->x = player->rect.x - CAMERA_MARGIN_X;

	if (player->camera->x < 0)
		player->camera->x = 0;
	else if (player->camera->x + player->camera->w > level_width * 16)
		player->camera->x = level_width * 16 - player->camera->w;

	if (player->rect.x < 0) {
		player->pos_x = 0.0f;
		player->rect.x = (int)player->pos_x;
	} else if (player->rect.x + player->rect.w > level_width * 16) {
		player->pos_x = level_width * 16 - player->rect.w;
		player->rect.x = (int)player->pos_x;
	}

	player->onground = 0;
	player->pos_y += player->vel_y * dt;
	player->rect.y = (int)player->pos_y;
	for (int i = 0; i < tile_vec->used; i++) {
		Tile tile = *tile_vec->vec[i];
		if (tile.collidable) {
			if (player->rect.y + player->rect.h >= tile.rect.y &&
				player->rect.x + player->rect.w > tile.rect.x &&
				player->rect.x < tile.rect.x + tile.rect.w &&
				player->rect.y < tile.rect.y)
			{
				player->vel_y = 0.0f;
				player->pos_y = tile.rect.y - player->rect.h;
				player->rect.y = (int)player->pos_y;
				player->onground = 1;
			}
			else if (player->rect.y < tile.rect.y + tile.rect.h &&
				player->rect.x + player->rect.w > tile.rect.x &&
				player->rect.x < tile.rect.x + tile.rect.w &&
				player->rect.y + player->rect.h > tile.rect.y + tile.rect.h)
			{
				player->vel_y = 0.0f;
				player->pos_y = tile.rect.y + tile.rect.h;
				player->rect.y = (int)player->pos_y;
			}
		}
	}

	if (player->rect.y + player->rect.h + CAMERA_MARGIN_Y > player->camera->y + player->camera->h) {
		player->camera->y = player->rect.y + player->rect.h + CAMERA_MARGIN_Y - player->camera->h;
	} else if (player->rect.y - CAMERA_MARGIN_Y < player->camera->y) {
		player->camera->y = player->rect.y - CAMERA_MARGIN_Y;
	}

	if (player->left && !player->right && !player->flip)
		player->flip = 1;
	else if (player->right && !player->left && player->flip)
		player->flip = 0;
}

void update_playing_state(Game *game, float dt)
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
			case SDLK_UP:
				if (game->player->onground)
					game->player->vel_y -= JUMP;
				break;
			case SDLK_LEFT:
				if (!game->player->left) {
					game->player->left = 1;
					game->player->vel_x -= SPEED;
				}
				break;
			case SDLK_RIGHT:
				if (!game->player->right) {
					game->player->right = 1;
					game->player->vel_x += SPEED;
				}
				break;
			case SDLK_z:
				game->player->state = attack;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				game->player->left = 0;
				game->player->vel_x += SPEED;
				break;
			case SDLK_RIGHT:
				game->player->right = 0;
				game->player->vel_x -= SPEED;
				break;
			case SDLK_z:
				game->player->state = neutral;
				break;
			}
		}
	}
	update_player(game->player, &game->tiles, game->level_width, dt);
}

void update_start_state(Game *game, float dt)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			game->state = quit;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
			case SDLK_SPACE:
				game->state = playing;
				break;
			}
			break;
		}
	}
}

void update_game(Game *game, float dt)
{
	switch (game->state) {
	case playing:
		update_playing_state(game, dt);
		break;
	case start:
		update_start_state(game, dt);
		break;
	}
}
