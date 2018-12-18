#ifndef RENDERING_H_
#define RENDERING_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"

static SDL_Texture *dirt_texture;
static SDL_Texture *grass_texture;
static SDL_Texture *player_texture;

void load_textures(SDL_Renderer *renderer);
void render_game(SDL_Renderer *renderer, const game_t *game);

#endif // RENDERING_H_
