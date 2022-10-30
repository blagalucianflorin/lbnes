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
#include <iomanip>

#include <SDL.h>

#include "bus/bus.h"
#include "devices/cpu/6502.h"
#include "devices/ppu/ppu.h"
#include "devices/cartridges/cartridge.h"
#include "devices/inputs/joypad.h"
#include "devices/memories/ram.h"
#include "devices/memories/ppu_nametable_ram.h"
#include "devices/memories/ppu_palette_ram.h"

#include "graphics/imgui_manager.hpp"
#include "graphics/sdl_manager.h"
#include "options/configurator.hpp"
#include "yaml-cpp/yaml.h"

#ifdef _WIN32
#include <windows.h>
#endif


class nes
{
private:
    friend class imgui_manager;

    SDL_Renderer *game_renderer   = nullptr;
    SDL_Window   *game_window     = nullptr;
    SDL_Event    game_input_event {};

    std::shared_ptr <ppu>       nes_ppu;
    std::shared_ptr <cpu>       nes_cpu;
    std::shared_ptr <bus>       cpu_bus;
    std::shared_ptr <bus>       ppu_bus;
    std::shared_ptr <ram>       cpu_ram;
    std::shared_ptr <cartridge> nes_cartridge;

    std::vector <std::unique_ptr <joypad>> joypads;
    std::shared_ptr <class imgui_manager>  imgui_manager;


    long long total_cycles = 0;
    double    target_fps   = 60.098814;
    bool      rom_loaded   = false;
    double    average_fps  = 60.098814;
    double    current_fps  = 60.098814;


    struct options
    {
        bool quit        = false;
        bool vsync       = false;
        bool display_fps;
        bool show_menu;
        bool fullscreen;
        int  speed;
    } options {};


    void load_joypads ();

    void process_events ();

    void sleep_until_next_frame (std::chrono::time_point<std::chrono::high_resolution_clock> &frame_start);

    void set_title ();

public:
    nes ();

    ~nes ();

    void     reset ();

    void     reload (const std::string& rom_file, bool initialize_controllers = false);

    void     start ();

    uint32_t *render_frame ();

    [[maybe_unused]] uint8_t set_button (joypad::BUTTON button, uint8_t value = 1, uint8_t player = 1);

    [[maybe_unused]] uint8_t get_button (joypad::BUTTON button, uint8_t player = 1);

    [[maybe_unused]] void    toggle_joypad (uint8_t player = 1);

    [[maybe_unused]] void    reset_buttons (uint8_t player = 1);

    static int SDLCALL       controller_connection_event_manager (void *userdata, SDL_Event * event);
};

#endif //NEMULATOR_NES_H
