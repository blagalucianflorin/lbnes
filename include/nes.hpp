//
// Created by lblaga on 27.05.2022.
//

#ifndef NES_HPP
#define NES_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <exception>
#include <memory>
#include <iomanip>
#include <array>
#include <functional>

#include <SDL.h>

#include "bus/bus.hpp"
#include "devices/cpu/6502.hpp"
#include "devices/ppu/ppu.hpp"
#include "devices/cartridges/cartridge.hpp"
#include "devices/inputs/joypad.hpp"
#include "devices/memories/ram.hpp"
#include "devices/memories/ppu_nametable_ram.hpp"
#include "devices/memories/ppu_palette_ram.hpp"

#include "graphics/imgui_manager.hpp"
#include "graphics/sdl_manager.hpp"
#include "options/configurator.hpp"
#include "yaml-cpp/yaml.h"
#include "misc/logger.hpp"
#include "misc/state.h"

#include "network/client.hpp"
#include "network/server.hpp"

#ifdef _WIN32
#include <windows.h>
#endif


class nes : public std::enable_shared_from_this <nes>, public state
{
public:
    nes ();

    virtual ~nes ();

    void start ();

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

    std::vector <std::shared_ptr <joypad>> joypads;
    std::shared_ptr <class imgui_manager>  imgui_manager;


    long long total_cycles = 0;
    double    target_fps   = 60.098814;
    bool      rom_loaded   = false;
    double    average_fps  = 60.098814;
    double    current_fps  = 60.098814;

    std::unique_ptr <class server> screen_server;
    std::unique_ptr <class client> screen_client;

    std::array <uint8_t, 240 * 256>  client_pixels_small {};
    std::array <uint32_t, 240 * 256> client_pixels {};
    SDL_Surface                      *client_screen_surface = nullptr;

    struct options
    {
        bool quit        = false;
        bool vsync       = false;
        bool display_fps {};
        bool show_menu {};
        bool fullscreen {};
        int  speed {};
        bool is_server = false;
        bool is_client = false;
        bool paused    = false;
    } options {};

    struct rgb_triplet
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    std::array <rgb_triplet, 64> color_palette {};


    void populate_palette_2C02 ();

    void load_joypads ();

    void process_events ();

    void sleep_until_next_frame (std::chrono::time_point<std::chrono::high_resolution_clock> &frame_start);


    void                    set_title_fps ();

    inline void             set_title_simple ()
    {
        SDL_SetWindowTitle (this -> game_window, "lbnes v" LBNES_VERSION);
    };

    std::function <void ()> set_title = std::bind (&nes::set_title_simple, this); // NOLINT


    void start_server ();

    void start_client ();


    void                                   emulate_frame_real ();

    void                                   emulate_frame_paused ();

    std::function <void ()>                emulate_frame = [] () {};

    const std::array <uint32_t, 240 * 256> &render_frame ();


    [[maybe_unused]] uint8_t set_button (joypad::BUTTON button, uint8_t value = 1, uint8_t player = 1);

    [[maybe_unused]] uint8_t get_button (joypad::BUTTON button, uint8_t player = 1);

    [[maybe_unused]] void    toggle_joypad (uint8_t player = 1);

    [[maybe_unused]] void    reset_buttons (uint8_t player = 1);

    static int SDLCALL       controller_connection_event_manager (void *userdata, SDL_Event * event);


    void reload (const std::string& rom_file, bool initialize_controllers = false);

    void reload_no_rom ();

    void reset ();


    std::string save_state() override;

    void        load_state (std::string saved_state) override;


    void handle_drop_file (const std::string& file_path);
};

#endif //NES_HPP
