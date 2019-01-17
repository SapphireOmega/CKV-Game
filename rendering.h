#ifndef RENDERING_H_
#define RENDERING_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"

static SDL_Texture *tile_sheet;

void load_textures(SDL_Renderer *renderer);
void render_game(SDL_Renderer *renderer, const Game *game);

#endif // RENDERING_H_
