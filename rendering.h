#ifndef RENDERING_H_
#define RENDERING_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "game.h"

static SDL_Texture *tile_sheet;
static TTF_Font *font;
static SDL_Surface *start_text_surface;
static SDL_Texture *start_text;
static SDL_Rect start_text_rect;

void load_text(SDL_Renderer *renderer, const GameWindow *game_window);
void load_textures(SDL_Renderer *renderer);

void render_game(SDL_Renderer *renderer, const Game *game);

void render_clean();

#endif // RENDERING_H_
