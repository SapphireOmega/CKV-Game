#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>

#include "entity.h"
#include "camera.h"

typedef struct {
	Entity *entity;
	Camera *camera;
	int left;
	int right;
	int flip;
	int attack;
	unsigned int attack_frame;
	Uint32 attack_start_tick;
	float death_timer;
	int next_level;
} Player;

Player *create_player(int x, int y, Camera *camera);

void destroy_player(Player *player);

#endif // PLAYER_H_
