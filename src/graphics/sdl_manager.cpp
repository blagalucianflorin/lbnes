//
// Created by lblaga on 22.05.2022.
//

#include "graphics/sdl_manager.hpp"


void toggle_fullscreen (SDL_Window* Window)
{
    bool IsFullscreen = SDL_GetWindowFlags (Window) & SDL_WINDOW_FULLSCREEN;
    SDL_SetWindowFullscreen (Window, IsFullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
    SDL_ShowCursor (IsFullscreen);
}


void create_sdl_window (SDL_Window *&window, SDL_Renderer *&renderer)
{
    int window_width  = configurator::get_instance ()["resolution"]["width"].as<int> ();
    int window_height = configurator::get_instance ()["resolution"]["height"].as<int> ();

    SDL_Init (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    SDL_CreateWindowAndRenderer (window_width, window_height, 0, &window, &renderer);
    SDL_SetWindowSize (window, window_width,window_height);
    SDL_SetWindowPosition (window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_RenderSetVSync (renderer, static_cast <int> (configurator::get_instance ()["vsync"].as <bool> () ? 1 : 0));

    if (configurator::get_instance ()["fullscreen"].as<bool> ())
        SDL_SetWindowFullscreen (window, SDL_WINDOW_FULLSCREEN);
}


void surface_set_pixel (SDL_Surface *surface, size_t x, size_t y, uint32_t pixel)
{
    auto old_pixel = reinterpret_cast <uint32_t*> (static_cast <uint8_t*> (surface -> pixels) +
                                                    (y * static_cast <size_t> (surface -> pitch)) +
                                                    (x * surface -> format -> BytesPerPixel));

    *old_pixel = pixel;
}


void update_vsync (SDL_Renderer *renderer)
{
    SDL_RenderSetVSync (renderer, static_cast <int> (configurator::get_instance ()["vsync"].as <bool> () ? 1 : 0));
}
