//
// Created by lblaga on 22.05.2022.
//

#ifndef NEMULATOR_SDL_MANAGER_H
#define NEMULATOR_SDL_MANAGER_H

#include "devices/ppu/ppu.h"
#include "options/configurator.hpp"

extern "C"
{
#include <SDL.h>
}

#include <iostream>


void toggle_fullscreen (SDL_Window* Window);

void create_sdl_window (SDL_Window *&window, SDL_Renderer *&renderer);

void surface_set_pixel (SDL_Surface *surface, size_t x, size_t y, uint32_t pixel);

void update_vsync (SDL_Renderer *renderer);

#endif //NEMULATOR_SDL_MANAGER_H
