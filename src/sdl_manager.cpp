//
// Created by lblaga on 22.05.2022.
//

#include "include/sdl_manager.h"


void ToggleFullscreen(SDL_Window* Window)
{
    bool IsFullscreen = SDL_GetWindowFlags(Window) & SDL_WINDOW_FULLSCREEN;
    SDL_SetWindowFullscreen(Window, IsFullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
    SDL_ShowCursor(IsFullscreen);
}

void main_window (SDL_Window *&window, SDL_Renderer *&renderer, const char *window_title,
                  int window_width, int window_height)
{
    SDL_Init (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    SDL_CreateWindowAndRenderer (window_width, window_height, 0, &window, &renderer);
    SDL_SetWindowSize (window, window_width * 3, window_height * 3);
    SDL_SetWindowPosition (window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowTitle (window, window_title);
//    TTF_Init();
}

void draw_tiles (ppu *my_ppu)
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

void draw_palette (SDL_Renderer *renderer, ppu *my_ppu)
{
    int top  = 300;
    int left = 900;
    int size = 10;

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

void draw_whole_palette (SDL_Renderer *renderer, ppu *my_ppu)
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


void surface_set_pixel (SDL_Surface *surface, int x, int y, uint32_t pixel)
{
    auto direct_access_pixel = (uint32_t*) (((uint8_t*) surface -> pixels) + (y * surface -> pitch) +
                               (x * surface -> format -> BytesPerPixel));

    *direct_access_pixel = pixel;
}