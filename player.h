#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>

#include "entity.h"
#include "camera.h"

typedef enum {
	player_neutral,
	player_attack,
} PlayerState;

typedef struct {
	Entity *entity;
	int left;
	int right;
	int flip;
	int attack;
	Camera *camera;
	PlayerState state;
	Uint32 attack_start_tick;
	unsigned int attack_frame;
	float death_timer;
} Player;

Player *create_player(int x, int y, Camera *camera);

void destroy_player(Player *player);

#endif // PLAYER_H_
