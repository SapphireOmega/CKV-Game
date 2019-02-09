#include "updating.h"

#define GRAV 1800 // 1800
#define SPEED 120 // 120
#define JUMP 400 // 400
#define CAMERA_MARGIN_X 160
#define CAMERA_MARGIN_Y 63
#define DEATH_TIME 300
#define PLAYER_ATTACK_RANGE 6
#define ENEMY_JUMP_RANGE 48
#define ENEMY_ATTACK_RANGE 192
#define ENEMY_SPEED 80
#define ENEMY_JUMP 300
#define KNOCK_BACK_SPEED 250
#define KNOCK_BACK_JUMP 300

int
update_entity_x(Entity *entity, TileVec *tiles, unsigned int level_width, float dt)
{
	int collided = 0;

	if (entity->move)
		entity->pos_x += entity->vel_x * dt;

	entity->rect.x = entity->pos_x;
	for (int i = 0; i < tiles->used; i++) {
		Tile tile = *tiles->vec[i];
		if (tile.collidable) {
			if (entity->rect.x + entity->rect.w > tile.rect.x &&
				entity->rect.y < tile.rect.y + tile.rect.h &&
				entity->rect.y + entity->rect.h > tile.rect.y &&
				entity->rect.x < tile.rect.x)
			{
				entity->pos_x = tile.rect.x - entity->rect.w;
				entity->rect.x = entity->pos_x;
				collided = 1;
			} else if (entity->rect.x < tile.rect.x + tile.rect.w &&
				entity->rect.y < tile.rect.y + tile.rect.h &&
				entity->rect.y + entity->rect.h > tile.rect.y &&
				entity->rect.x + entity->rect.w > tile.rect.x + tile.rect.w)
			{
				entity->pos_x = tile.rect.x + tile.rect.w;
				entity->rect.x = entity->pos_x;
				collided = 1;
			}
		}
	}

	if (entity->rect.x < 0) {
		entity->pos_x = 0.0f;
		entity->rect.x = entity->pos_x;
		collided = 1;
	} else if (entity->rect.x + entity->rect.w > level_width * 16) {
		entity->pos_x = level_width * 16 - entity->rect.w;
		entity->rect.x = entity->pos_x;
		collided = 1;
	}

	return collided;
}

int
update_entity_y(Entity *entity, TileVec *tiles, unsigned int level_height, float dt)
{
	int collided = 0;

	entity->vel_y += GRAV * dt;

	entity->on_ground = 0;
	entity->pos_y += entity->vel_y * dt;
	entity->rect.y = entity->pos_y;
	for (int i = 0; i < tiles->used; i++) {
		Tile tile = *tiles->vec[i];
		if (tile.collidable) {
			if (entity->rect.y + entity->rect.h >= tile.rect.y &&
				entity->rect.x + entity->rect.w > tile.rect.x &&
				entity->rect.x < tile.rect.x + tile.rect.w &&
				entity->rect.y < tile.rect.y)
			{
				entity->vel_y = 0.0f;
				entity->pos_y = tile.rect.y - entity->rect.h;
				entity->rect.y = entity->pos_y;
				entity->on_ground = 1;
				collided = 1;
			} else if (entity->rect.y < tile.rect.y + tile.rect.h &&
				entity->rect.x + entity->rect.w > tile.rect.x &&
				entity->rect.x < tile.rect.x + tile.rect.w &&
				entity->rect.y + entity->rect.h > tile.rect.y + tile.rect.h)
			{
				entity->vel_y = 0.0f;
				entity->pos_y = tile.rect.y + tile.rect.h;
				entity->rect.y = entity->pos_y;
				collided = 1;
			}
		}
	}

	if (entity->rect.y > level_height * 16) {
		entity->alive = 0;
		collided = 1;
	}

	return collided;
}

void
update_player_camera_x(Player *player, unsigned int level_width, unsigned int level_height)
{
	if (player->entity->rect.x + player->entity->rect.w + CAMERA_MARGIN_X > player->camera->x + player->camera->w)
		player->camera->x = player->entity->rect.x + player->entity->rect.w + CAMERA_MARGIN_X - player->camera->w;
	else if (player->entity->rect.x - CAMERA_MARGIN_X < player->camera->x)
		player->camera->x = player->entity->rect.x - CAMERA_MARGIN_X;

	if (player->camera->x < 0)
		player->camera->x = 0;
	else if (player->camera->x + player->camera->w > level_width * 16)
		player->camera->x = level_width * 16 - player->camera->w;
}

void
update_player_camera_y(Player *player, unsigned int level_width, unsigned int level_height)
{
	if (player->entity->rect.y + player->entity->rect.h + CAMERA_MARGIN_Y > player->camera->y + player->camera->h) {
		player->camera->y = player->entity->rect.y + player->entity->rect.h + CAMERA_MARGIN_Y - player->camera->h;
	} else if (player->entity->rect.y - CAMERA_MARGIN_Y < player->camera->y) {
		player->camera->y = player->entity->rect.y - CAMERA_MARGIN_Y;
	}

	if (player->camera->y + player->camera->h > level_height * 16 - 1)
		player->camera->y = level_height * 16 - player->camera->h - 1;
}

void
update_player(Player *player, TileVec *tiles, unsigned int level_width, unsigned int level_height, GameState *game_state, float dt)
{
	if (player->state == player_attack)
		player->entity->move = 0;
	else
		player->entity->move = 1;

	if (player->left && !player->right)
		player->entity->vel_x = -SPEED;
	else if (!player->left && player->right)
		player->entity->vel_x = SPEED;
	else
		player->entity->vel_x = 0;

	update_entity_x(player->entity, tiles, level_width, dt);
	update_player_camera_x(player, level_width, level_height);
	update_entity_y(player->entity, tiles, level_height, dt);
	update_player_camera_y(player, level_width, level_height);

	if (player->left && !player->right && !player->flip && player->state == player_neutral)
		player->flip = 1;
	else if (player->right && !player->left && player->flip && player->state == player_neutral)
		player->flip = 0;

	if (player->state == player_attack) {
		Uint32 current_tick = SDL_GetTicks();
		Uint32 delta_ticks = current_tick - player->attack_start_tick;
		if (delta_ticks >= 0 && delta_ticks < 20) {
			player->attack_frame = 1;
			player->attack = 1;
		} else if (delta_ticks >= 20 && delta_ticks < 200)
			player->attack_frame = 2;
		else if (delta_ticks >= 200 && delta_ticks < 220)
			player->attack_frame = 1;
		else if (delta_ticks >= 220 && delta_ticks < 400) {
			player->attack_frame = 0;
			player->attack = 0;
		} else {
			player->state = player_neutral;
		}
	}

	if (player->entity->hp <= 0)
		player->entity->alive = 0;

	if (!player->entity->alive)
		*game_state = death;
}

void
update_enemies(EnemyVec *enemies, TileVec *tiles, Player *player, unsigned int level_width, unsigned int level_height, float dt)
{
	for (int e = 0; e < enemies->used; e++) {
		Enemy *enemy = enemies->vec[e];
	
		int x = enemy->entity->rect.x + enemy->entity->rect.w / 2 - player->entity->rect.x - player->entity->rect.w / 2;
		if (x <= ENEMY_ATTACK_RANGE && x > 0 && !enemy->entity->knock_back) {
			enemy->entity->vel_x = -ENEMY_SPEED;
			enemy->entity->flip = 1;
		} else if (x >= -ENEMY_ATTACK_RANGE && x < 0 && !enemy->entity->knock_back) {
			enemy->entity->vel_x = ENEMY_SPEED;
			enemy->entity->flip = 0;
		}

		if (enemy->state == enemy_attack)
			enemy->entity->move = 0;
		else
			enemy->entity->move = 1;

		if (update_entity_x(enemy->entity, tiles, level_width, dt) && enemy->entity->on_ground) {
			enemy->entity->vel_y = -ENEMY_JUMP;
			enemy->entity->knock_back = 0;
		}

		update_entity_y(enemy->entity, tiles, level_height, dt);

		if (player->attack) {
			if (player->flip &&
				!enemy->hit &&
				enemy->entity->rect.x + enemy->entity->rect.w > player->entity->rect.x - PLAYER_ATTACK_RANGE &&
				enemy->entity->rect.x < player->entity->rect.x)
			{
				enemy->entity->hp -= 1;
				enemy->entity->knock_back = 1;
				enemy->entity->vel_y = -KNOCK_BACK_JUMP;
				enemy->entity->vel_x = -KNOCK_BACK_SPEED;
				enemy->hit = 1;
			} else if (!player->flip &&
				!enemy->hit &&
				enemy->entity->rect.x < player->entity->rect.x + player->entity->rect.w + PLAYER_ATTACK_RANGE &&
				enemy->entity->rect.x > player->entity->rect.x)
			{
				enemy->entity->hp -= 1;
				enemy->entity->knock_back = 1;
				enemy->entity->vel_y = -KNOCK_BACK_JUMP;
				enemy->entity->vel_x = KNOCK_BACK_SPEED;
				enemy->hit = 1;
			} else if (player->flip &&
				enemy->hit &&
				enemy->entity->rect.x + enemy->entity->rect.w < player->entity->rect.x - PLAYER_ATTACK_RANGE)
			{
				enemy->hit = 0;
			} else if (!player->flip &&
				enemy->hit &&
				enemy->entity->rect.x > player->entity->rect.x + player->entity->rect.w + PLAYER_ATTACK_RANGE)
			{
				enemy->hit = 0;
			}
		}

		if (enemy->entity->knock_back && enemy->entity->vel_y == 0)
			enemy->entity->knock_back = 0;

		if (enemy->entity->hp <= 0)
			enemy->entity->alive = 0;

		if (enemy->entity->alive &&
			!enemy->hit_player &&
			enemy->entity->rect.x < player->entity->rect.x + player->entity->rect.w &&
			enemy->entity->rect.x + enemy->entity->rect.w > player->entity->rect.x &&
			enemy->entity->rect.y < player->entity->rect.y + player->entity->rect.h &&
			enemy->entity->rect.y + player->entity->rect.h > player->entity->rect.y)
		{
			player->entity->hp -= 1;
			//player->entity->knock_back = 1;
			enemy->hit_player= 1;
		} else if (enemy->hit_player &&
			(enemy->entity->rect.x > player->entity->rect.x + player->entity->rect.w ||
			 enemy->entity->rect.x + enemy->entity->rect.w < player->entity->rect.x ||
			 enemy->entity->rect.y > player->entity->rect.y + player->entity->rect.h ||
			 enemy->entity->rect.y + player->entity->rect.h < player->entity->rect.y))
		{
			enemy->hit_player = 0;
		}
	}
}

void
update_playing_state(Game *game, float dt)
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
				if (game->player->entity->on_ground && game->player->state != player_attack)
					game->player->entity->vel_y -= JUMP;
				break;
			case SDLK_LEFT:
				//if (!game->player->left) {
					game->player->left = 1;
					//game->player->entity->vel_x -= SPEED;
				//}
				break;
			case SDLK_RIGHT:
				//if (!game->player->right) {
					game->player->right = 1;
					//game->player->entity->vel_x += SPEED;
				//}
				break;
			case SDLK_z:
				if (game->player->state == player_neutral && game->player->entity->vel_y == 0.0f) {
					game->player->state = player_attack;
					game->player->attack_start_tick = SDL_GetTicks();
				}
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				//if (game->player->left) {
					game->player->left = 0;
					//game->player->entity->vel_x += SPEED;
				//}
				break;
			case SDLK_RIGHT:
				//if (game->player->right) {
					game->player->right = 0;
					//game->player->entity->vel_x -= SPEED;
				//}
				break;
			}
		}
	}

	update_enemies(&game->enemies, &game->tiles, game->player, game->level_width, game->level_height, dt);
	update_player(game->player, &game->tiles, game->level_width, game->level_height, &game->state, dt);
}

void
update_start_state(Game *game, float dt)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			game->state = quit;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
			case SDLK_ESCAPE:
				game->state = quit;
				break;
			case SDLK_SPACE:
				game->state = playing;
				break;
			}
			break;
		}
	}
}

void
update_death_state(Game *game, float dt)
{
	Player *player = game->player;
	player->death_timer += dt * 100.0f;
	if (player->death_timer >= DEATH_TIME) {
		game->state = playing;
		player->death_timer = 0.0f;
		player->left = 0;
		player->right = 0;
		respawn_entity(player->entity);
		update_player_camera_x(player, game->level_width, game->level_height);
		update_player_camera_y(player, game->level_width, game->level_height);
		for (int x = 0; x < game->enemies.used; x++) {
			Enemy *enemy = game->enemies.vec[x];
			respawn_entity(enemy->entity);
		}
	}
}

void
update_game(Game *game, float dt)
{
	switch (game->state) {
	case playing:
		update_playing_state(game, dt);
		break;
	case start:
		update_start_state(game, dt);
		break;
	case death:
		update_death_state(game, dt);
		break;
	}
}
