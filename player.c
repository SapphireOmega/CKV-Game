#include "player.h"

Player *
create_player(int x, int y, Camera* camera)
{
	Player *player = (Player*)malloc(sizeof(Player));
	if (!player)
		return NULL;

	player->entity = create_entity(x, y, 15, 16, 3);
	player->left = 0;
	player->right = 0;
	player->flip = 0;
	player->attack = 0;
	player->camera = camera;
	player->attack_start_tick = 0;
	player->death_timer = 0.0f;

	return player;
}

void
destroy_player(Player *player)
{
	free(player->camera);
	free(player->entity);
	free(player);
}
