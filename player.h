#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>

typedef struct {
	SDL_Rect collider;
	SDL_Rect render_rect;
	int x_render_offset;
	int y_render_offset;
	float pos_x;
	float pos_y;
	float vel_x;
	float vel_y;
	int onground;
	int left;
	int right;
} player_t;

player_t *create_player(int x, int y);

#endif // PLAYER_H_
