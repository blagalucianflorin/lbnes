//
// Created by lblaga on 27.05.2022.
//

#ifndef NEMULATOR_NES_H
#define NEMULATOR_NES_H

#ifdef INSPECT
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#else
#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 240
#endif

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <exception>

#include <SDL2/SDL.h>

#include "include/bus/bus.h"
#include "include/devices/cpu/6502.h"
#include "include/devices/ppu/ppu.h"
#include "include/devices/cartridges/cartridge.h"
#include "include/devices/inputs/joypad.h"
#include "include/devices/memories/ram.h"
#include "include/devices/memories/ppu_nametable_ram.h"
#include "include/devices/memories/ppu_palette_ram.h"

#include "include/sdl_manager.h"

class nes
{
private:
    SDL_Renderer *game_renderer   = nullptr;
    SDL_Window   *game_window     = nullptr;
    SDL_Event    game_input_event {};

    std::unique_ptr <ppu> nes_ppu;
    cpu                   nes_cpu;
    bus                   cpu_bus;
    bus                   ppu_bus;

    ram             cpu_ram;
    ppu_palette_ram palette_ram;
    cartridge       nes_cartridge;
    joypad          player_one_joypad;
    joypad          player_two_joypad;

    long long total_cycles = 0;
    double    target_fps   = 60.098814;

    void main_loop ();

public:
    explicit nes (const std::string& rom_file);

    ~nes ();

    void        reset ();

    inline void toggle_fullscreen () { ToggleFullscreen (this -> game_window); }

    void        start ();

    uint32_t    *render_frame ();

    uint8_t     set_button (joypad::BUTTON button, uint8_t value = 1, uint8_t player = 1);

    uint8_t     get_button (joypad::BUTTON button, uint8_t player = 1);

    void        toggle_joypad (uint8_t player = 1);

    void        reset_buttons (uint8_t player = 1);
};

#endif //NEMULATOR_NES_H
