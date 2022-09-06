//
// Created by lblaga on 27.05.2022.
//

#ifndef NEMULATOR_NES_H
#define NEMULATOR_NES_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <exception>
#include <memory>

#include "bus/bus.h"
#include "devices/cpu/6502.h"
#include "devices/ppu/ppu.h"
#include "devices/cartridges/cartridge.h"
#include "devices/inputs/joypad.h"
#include "devices/memories/ram.h"
#include "devices/memories/ppu_nametable_ram.h"
#include "devices/memories/ppu_palette_ram.h"

#ifdef _WIN32
#include <SDL.h>
#include <windows.h>
#else
#include <SDL2/SDL.h>
#endif

#include "sdl_manager.h"
#include "configurator.h"
#include "yaml-cpp/yaml.h"

class nes
{
private:
    SDL_Renderer *game_renderer   = nullptr;
    SDL_Window   *game_window     = nullptr;
    SDL_Event    game_input_event {};

    std::shared_ptr <ppu> nes_ppu;
    std::shared_ptr <cpu> nes_cpu;
    std::shared_ptr <bus> cpu_bus;
    std::shared_ptr <bus> ppu_bus;

    std::shared_ptr <ram>             cpu_ram;
    std::shared_ptr <ppu_palette_ram> palette_ram;
    std::shared_ptr <cartridge>       nes_cartridge;

    std::vector<std::unique_ptr<joypad>> joypads;

    long long total_cycles = 0;
    double    target_fps   = 60.098814;

    void main_loop ();

    void load_joypads ();

public:
    explicit nes (const std::string& rom_file);

    ~nes ();

    void        reset ();

    void        reload (const std::string& rom_file);

    inline void toggle_fullscreen () { ToggleFullscreen (this -> game_window); }

    void        start ();

    uint32_t    *render_frame ();

    uint8_t     set_button (joypad::BUTTON button, uint8_t value = 1, uint8_t player = 1);

    uint8_t     get_button (joypad::BUTTON button, uint8_t player = 1);

    void        toggle_joypad (uint8_t player = 1);

    void        reset_buttons (uint8_t player = 1);

    static int SDLCALL controller_connection_event_manager (void *userdata, SDL_Event * event);
};

#endif //NEMULATOR_NES_H
