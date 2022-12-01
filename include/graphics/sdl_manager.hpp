//
// Created by lblaga on 22.05.2022.
//

#ifndef SDL_MANAGER_HPP
#define SDL_MANAGER_HPP

extern "C"
{
#include <SDL.h>
}

#include <iostream>

#include "devices/ppu/ppu.hpp"
#include "options/configurator.hpp"


void toggle_fullscreen (SDL_Window* Window);

void create_sdl_window (SDL_Window *&window, SDL_Renderer *&renderer);

void surface_set_pixel (SDL_Surface *surface, size_t x, size_t y, uint32_t pixel);

void update_vsync (SDL_Renderer *renderer);

#endif //SDL_MANAGER_HPP
