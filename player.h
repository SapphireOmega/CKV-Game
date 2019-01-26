#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>

#include "camera.h"

typedef enum {
	player_neutral,
	player_attack,
} PlayerState;

typedef struct {
	SDL_Rect rect;
	float pos_x;
	float pos_y;
	float vel_x;
	float vel_y;
	int onground;
	int left;
	int right;
	int flip;
	Camera* camera;
	PlayerState state;
	Uint32 attack_start_tick;
	unsigned int attack_frame;
} Player;

Player *create_player(int x, int y, Camera *camera);

void destroy_player(Player *player);

#endif // PLAYER_H_
