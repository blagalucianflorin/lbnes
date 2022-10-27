//
// Created by blaga on 26-Oct-22.
//

#ifndef LBNES_IMGUI_MANAGER_HPP
#define LBNES_IMGUI_MANAGER_HPP

#include <SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "portable-file-dialogs.h"

#include "nes.h"

class imgui_manager
{
private:
    SDL_Renderer *renderer;
    SDL_Window   *window;
    class nes    *nes;

public:
    imgui_manager (SDL_Renderer *renderer, SDL_Window *window, class nes *nes);

    ~imgui_manager ();

    void draw_menu ();

    static void process_event (SDL_Event *event);
};

#endif //LBNES_IMGUI_MANAGER_HPP
