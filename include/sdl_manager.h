//
// Created by lblaga on 22.05.2022.
//

#ifndef NEMULATOR_SDL_MANAGER_H
#define NEMULATOR_SDL_MANAGER_H

#include "devices/ppu/ppu.h"

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <iostream>


void ToggleFullscreen(SDL_Window* Window);

void main_window (SDL_Window *&window, SDL_Renderer *&renderer, const char *window_title,
                  int window_width = 256, int window_height = 240);

void draw_tiles (ppu *my_ppu);

void draw_palette (SDL_Renderer *renderer, ppu *my_ppu);

void draw_whole_palette (SDL_Renderer *renderer, ppu *my_ppu);

void surface_set_pixel (SDL_Surface *surface, int x, int y, uint32_t pixel);

#endif //NEMULATOR_SDL_MANAGER_H
