#include "player.h"

player_t *create_player(int x, int y, int w, int h)
{
	player_t *player = (player_t*)malloc(sizeof(player_t));
	if (!player)
		return NULL;

	player->rect.x = x;
	player->rect.y = y;
	player->rect.w = w;
	player->rect.h = h;
	player->pos_x = x;
	player->pos_y = y;
	player->vel_x = 0.0f;
	player->vel_y = 0.0f;
	player->onground = 0;
	player->left = 0;
	player->right = 0;

	return player;
}
