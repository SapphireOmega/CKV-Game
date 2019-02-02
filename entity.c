#include "entity.h"

Entity *
create_entity(int x, int y, int w, int h)
{
	Entity *entity = (Entity*)malloc(sizeof(Entity));

	entity->spawn_x = x;
	entity->spawn_y = y;
	entity->pos_x = x;
	entity->pos_y = y;
	entity->spawn_vel_y = 0.0f;
	entity->spawn_vel_y = 0.0f;
	entity->vel_x = 0.0f;
	entity->vel_y = 0.0f;
	entity->rect.x = x;
	entity->rect.y = y;
	entity->rect.w = w;
	entity->rect.h = h;
	entity->on_ground = 0;
	entity->flip = 0;
	entity->spawn_flip = 0;
	entity->alive = 1;
	entity->move = 1;

	return entity;
}
