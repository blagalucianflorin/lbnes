//
// Created by lblaga on 22.05.2022.
//

#include "graphics/sdl_manager.h"


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


#ifdef DEBUG
[[maybe_unused]] void draw_tiles (ppu *my_ppu)
{
    int top = 10;
    int left = 810;
    int new_row = 0;

    std::map <int, std::vector <int>> palette =
            {
                    {0, {0, 0, 0}},
                    {1, {255, 0, 0}},
                    {2, {0, 255, 0}},
                    {3, {0, 0, 255}},
            };

    for (int i = 0; i < 512; i++)
    {
        my_ppu -> draw_tile (i * 16, left, top);

        left += 8;
        new_row++;
        if (new_row == 16)
        {
            top += 8;
            new_row = 0;
            left = 810;
        }
    }
}


[[maybe_unused]] void draw_palette (SDL_Renderer *renderer, ppu *my_ppu)
{
    int top  = 0;
    int left = 0;
    int size = 25;

    for (uint16_t i = 0; i < 8; i++)
    {
        for (uint16_t j = 0; j < 4; j++)
        {
            ppu::rgb_triplet color = (my_ppu -> color_palette)[my_ppu -> read (0x3F00 + (i * 4) + j, false)];
            SDL_Rect rect;

            rect.x = left + j * size;
            rect.y = top + i * size;
            rect.h = size;
            rect.w = size;

            SDL_SetRenderDrawColor (renderer, color.r, color.g, color.b, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}


[[maybe_unused]] void draw_whole_palette (SDL_Renderer *renderer, ppu *my_ppu)
{

    int top  = 600;
    int left = 100;
    int size = 10;

    for (uint16_t i = 0; i < 4; i++)
    {
        for (uint16_t j = 0; j < 16; j++)
        {
            ppu::rgb_triplet color = (my_ppu -> color_palette)[i * 16 + j];
            SDL_Rect rect;

            rect.x = left + j * size;
            rect.y = top + i * size;
            rect.h = size;
            rect.w = size;

            SDL_SetRenderDrawColor (renderer, color.r, color.g, color.b, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
#endif