#include "enemy.h"

Enemy *
create_enemy(int x, int y, EnemyType type)
{
	Enemy *enemy = (Enemy*)malloc(sizeof(Enemy));
	if (!enemy)
		return NULL;

	enemy->entity = create_entity(x, y, 12, 16, 3);
	enemy->entity->vel_x = 0;
	enemy->entity->spawn_vel_x = 0;
	enemy->state = enemy_neutral;
	enemy->type = type;
	enemy->hit_player = 0;
	enemy->hit = 0;

	return enemy;
}

int
init_enemy_vec(EnemyVec *enemy_vec, size_t init_size)
{
	enemy_vec->vec = (Enemy**)malloc(init_size*sizeof(Enemy*));
	if (!enemy_vec->vec)
		return 1;
	enemy_vec->used = 0;
	enemy_vec->size = init_size;
	return 0;
}

int
push_enemy_vec(EnemyVec *enemy_vec, Enemy *enemy)
{
	if (enemy_vec->used == enemy_vec->size) {
		enemy_vec->size *= 2;
		enemy_vec->vec = (Enemy**)realloc(enemy_vec->vec,enemy_vec->size*sizeof(Enemy*));
		if (!enemy_vec->vec)
			return 1;
	}
	enemy_vec->vec[enemy_vec->used++] = enemy;
	return 0;
}

void
empty_enemy_vec(EnemyVec *enemy_vec, size_t init_size)
{
	for (int i = 0; i < enemy_vec->used; i++)
		free(enemy_vec->vec[i]);
	enemy_vec->used = 0;
	enemy_vec->size = init_size;
	enemy_vec->vec = (Enemy**)realloc(enemy_vec->vec, enemy_vec->size*sizeof(Enemy*));
}

void
free_enemy_vec(EnemyVec *enemy_vec)
{
	for (int i = 0; i < enemy_vec->used; i++)
		free(enemy_vec->vec[i]);
	free(enemy_vec->vec);
	enemy_vec->vec = NULL;
	enemy_vec->used = 0;
	enemy_vec->size = 0;
}
