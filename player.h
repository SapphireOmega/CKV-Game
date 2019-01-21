#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>

#include "camera.h"

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
} Player;

Player *create_player(int x, int y, Camera *camera);

#endif // PLAYER_H_
