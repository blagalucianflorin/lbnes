//
// Created by blaga on 26-Oct-22.
//

#ifndef LBNES_IMGUI_MANAGER_HPP
#define LBNES_IMGUI_MANAGER_HPP

#include "forwards/classes.hpp"
#include "nes.hpp"

#include <SDL.h>

#include <memory>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "portable-file-dialogs.h"
#include "options/configurator.hpp"
#include "misc/logger.hpp"


class imgui_manager
{
public:
    imgui_manager (SDL_Renderer *renderer, SDL_Window *window, std::shared_ptr <class nes> new_nes);

    ~imgui_manager ();

    void        draw_menu ();

    static void process_event (SDL_Event *event);

private:
    SDL_Renderer                *renderer;
    SDL_Window                  *window;
    std::shared_ptr <class nes> my_nes;
};

#endif //LBNES_IMGUI_MANAGER_HPP
