#include "player.h"

Player *create_player(int x, int y, Camera* camera)
{
	Player *player = (Player*)malloc(sizeof(Player));
	if (!player)
		return NULL;

	player->rect.x = x;
	player->rect.y = y;
	player->rect.w = 15;
	player->rect.h = 16;
	player->pos_x = x;
	player->pos_y = y;
	player->vel_x = 0.0f;
	player->vel_y = 0.0f;
	player->onground = 0;
	player->left = 0;
	player->right = 0;
	player->flip = 0;
	player->camera = camera;
	player->state = neutral;
	player->attack_start_tick = 0;

	return player;
}

void destroy_player(Player *player)
{
	destroy_camera(player->camera);
	free(player);
}
