#include "player.h"

player_t *create_player(int x, int y)
{
	player_t *player = (player_t*)malloc(sizeof(player_t));
	if (!player)
		return NULL;

	player->render_rect.x = x;
	player->render_rect.y = y;
	player->render_rect.w = 16;
	player->render_rect.h = 18;
	player->x_render_offset = 3;
	player->y_render_offset = 2;
	player->collider.x = x + player->x_render_offset;
	player->collider.y = y + player->y_render_offset;
	player->collider.w = 9;
	player->collider.h = 15;
	player->pos_x = player->collider.x;
	player->pos_y = player->collider.y;
	player->vel_x = 0.0f;
	player->vel_y = 0.0f;
	player->onground = 0;
	player->left = 0;
	player->right = 0;

	return player;
}
