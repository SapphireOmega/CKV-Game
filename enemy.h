#ifndef ENEMY_H_
#define ENEMY_H_

#include "entity.h"

#include <SDL2/SDL.h>

typedef enum {
	dark_magician,
} EnemyType;

typedef struct {
	Entity *entity;
	EnemyType type;
	int melee_cooldown;
	int melee_start_tick;
	int hit_player;
	int hit;
} Enemy;

Enemy *create_enemy(int x, int y, EnemyType type);

typedef struct {
	Enemy **vec;
	size_t used;
	size_t size;
} EnemyVec;

int init_enemy_vec(EnemyVec *enemy_vec, size_t init_size);
int push_enemy_vec(EnemyVec *enemy_vec, Enemy *enemy);
void empty_enemy_vec(EnemyVec *enemy_vec, size_t init_size);
void free_enemy_vec(EnemyVec *enemy_vec);

#endif // ENEMY_H_
