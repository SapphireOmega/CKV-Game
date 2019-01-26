#ifndef ENEMY_H_
#define ENEMY_H_

#include <SDL2/SDL.h>

typedef enum {
	enemy_neutral,
	enemy_attack,
} EnemyState;

typedef enum {
	dark_magician,
} EnemyType;

typedef struct {
	SDL_Rect rect;
	float pos_x;
	float pos_y;
	float vel_x;
	float vel_y;
	int flip;
	int on_ground;
	EnemyState state;
	EnemyType type;
} Enemy;

Enemy *create_enemy(int x, int y, EnemyType type);

typedef struct {
	Enemy **vec;
	size_t used;
	size_t size;
} EnemyVec;

int init_enemy_vec(EnemyVec *enemy_vec, size_t init_size);
int push_enemy_vec(EnemyVec *enemy_vec, Enemy *enemy);
int pop_enemy_vec(EnemyVec *enemy_vec);
void empty_enemy_vec(EnemyVec *enemy_vec, size_t init_size);
void free_enemy_vec(EnemyVec *enemy_vec);

#endif // ENEMY_H_
