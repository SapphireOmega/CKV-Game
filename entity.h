#ifndef ENTITY_H_
#define ENTITY_H_

#include <SDL2/SDL.h>

typedef enum {
	neutral,
	attack,
	melee,
	knock_back,
	jump,
	block,
} EntityState;

typedef struct {
	SDL_Rect rect;
	EntityState state;
	float pos_x;
	float pos_y;
	float vel_x;
	float vel_y;
	int on_ground;
	int flip;
	int alive;
	int move;
	int hp;
	int spawn_hp;
	float spawn_x;
	float spawn_y;
	float spawn_vel_x;
	float spawn_vel_y;
	int spawn_flip;
} Entity;

Entity *create_entity(int x, int y, int w, int h, int hp);
void respawn_entity(Entity *entity);

#endif // ENTITY_H_
