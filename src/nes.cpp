//
// Created by lblaga on 27.05.2022.
//

#include "nes.hpp"


nes::nes ()
{
    auto rom_file = configurator::get_instance ()["rom"].as <std::string> ();

    LOGGER_INFO ("Creating SDL window and renderer.");
    create_sdl_window (this -> game_window, this -> game_renderer);
    SDL_SetEventFilter (nes::controller_connection_event_manager, &(this -> joypads));

    this -> options.display_fps = configurator::get_instance ()["display_fps"].as <bool> ();
    this -> options.fullscreen  = configurator::get_instance ()["fullscreen"].as <bool> ();
    this -> options.show_menu   = configurator::get_instance ()["show_menu"].as <bool> ();
    this -> options.vsync       = configurator::get_instance ()["vsync"].as <bool> ();
    this -> options.speed       = configurator::get_instance ()["speed"].as <int> ();
    this -> options.quit        = false;

    this -> populate_palette_2C02 ();

    if (configurator::get_instance ()["server"].as <bool> ())
        this -> start_server ();

    if (configurator::get_instance ()["client"].as <bool> ())
        this -> start_client ();

    LOGGER_INFO ("Trying to load '" + rom_file + "' ROM file.");
    if (!rom_file.empty ())
        this -> reload (rom_file, true);
}


nes::~nes ()
{
    LOGGER_INFO ("Deleting my_nes.");

    this -> imgui_manager.reset ();

    SDL_DestroyRenderer (this -> game_renderer);
    SDL_DestroyWindow (this -> game_window);
    SDL_Quit ();
}


void nes::start ()
{
    LOGGER_INFO ("Creating ImGui manager.");
    this -> imgui_manager = std::make_shared <class imgui_manager> (this -> game_renderer, this -> game_window,
                                                                    shared_from_this ());

    LOGGER_INFO ("Starting emulation.");

    auto frame_start = std::chrono::high_resolution_clock::now ();

    if (this -> options.is_server)
        this -> joypads[1] -> toggle_activated ();

    if (this -> options.is_client)
        this -> joypads[0] -> toggle_activated ();

    while (!this -> options.quit)
    {
        frame_start = std::chrono::high_resolution_clock::now();

        this -> process_events ();
        this -> render_frame ();
        this -> sleep_until_next_frame (frame_start);
        this -> set_title ();
    }
}


void nes::reset ()
{
    if (!this -> rom_loaded)
        return;

    LOGGER_INFO ("Resetting my_nes.");

    this -> nes_cpu -> reset ();
    this -> nes_cartridge -> reset ();
    this -> nes_ppu -> reset ();
}


void nes::emulate_frame_real ()
{
    // NOT CLOCK CYCLE ACCURATE
    size_t clocks = (262 * 341) + (this -> nes_ppu -> is_odd_frame () ? 1 : 0);

    for (size_t i = 0; i < clocks; i += 3)
    {
        this -> nes_ppu -> clock ();
        this -> nes_ppu -> clock ();
        this -> nes_ppu -> clock ();

        this -> nes_cpu -> clock ();

        this -> total_cycles += 3;
    }
}


const std::array <uint32_t, 240 * 256> &nes::render_frame ()
{
    SDL_SetRenderDrawColor (this -> game_renderer, 128, 128, 128, 255);
    SDL_RenderClear (this -> game_renderer);

    this -> emulate_frame ();

    this -> imgui_manager -> draw_menu ();
    SDL_RenderPresent (this -> game_renderer);

    return (this -> nes_ppu -> get_pixels ());
}


[[maybe_unused]] uint8_t nes::get_button (joypad::BUTTON button, uint8_t player)
{
    return ((this -> joypads)[player] -> get_button (button));
}


[[maybe_unused]] uint8_t nes::set_button (joypad::BUTTON button, uint8_t value, uint8_t player)
{
    return ((this -> joypads)[player] -> set_button (button, value));
}


[[maybe_unused]] void nes::reset_buttons (uint8_t player)
{
    (this -> joypads)[player] -> reset_buttons ();

    LOGGER_INFO ("Reset joypad buttons for player '" + std::to_string (static_cast <int> (player)) + "'.");
}


[[maybe_unused]] void nes::toggle_joypad (uint8_t player)
{
    (this -> joypads)[player] -> toggle_activated ();

    LOGGER_INFO ("Toggled joypad for player '" + std::to_string (static_cast <int> (player)) + "'.");
}


void nes::load_joypads ()
{
    LOGGER_INFO ("Initializing joypads.");

    auto players = configurator::get_instance ()["joypads"];
    for (auto player : players)
    {
        if (player["type"].as<std::string>() == "keyboard")
        {
            std::vector<std::string> player_one_mapping;
            player_one_mapping.push_back (player["mapping"]["dpad-right"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["dpad-left"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["dpad-down"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["dpad-up"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["start"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["select"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["a"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["b"].as<std::string>());

            (this -> joypads)[static_cast <size_t> (player["player"].as<int>() - 1)] -> change_type (joypad::KEYBOARD);
            (this -> joypads)[static_cast <size_t> (player["player"].as<int>() - 1)] -> set_mapping (player_one_mapping);
            (this -> joypads)[static_cast <size_t> (player["player"].as<int>() - 1)] ->
                    change_player_number (player["player"].as<int>());

            LOGGER_INFO ("Loaded keyboard joypad for player '" + std::to_string (player["player"].as<int>()) + "'.");
        }
        else
        {
            (this -> joypads)[static_cast <size_t> (player["player"].as<int>() - 1)] ->
                    change_controller_number (player["controller_index"].as<int>() - 1);
            (this -> joypads)[static_cast <size_t> (player["player"].as<int>() - 1)] -> change_type (joypad::CONTROLLER);
            (this -> joypads)[static_cast <size_t> (player["player"].as<int>() - 1)] ->
                    change_player_number (player["player"].as<int>());

            LOGGER_INFO ("Loaded controller joypad for player '" + std::to_string (player["player"].as<int>()) + "'.");
        }
    }
}


int nes::controller_connection_event_manager (void *user_data, SDL_Event *event)
{
    if (!configurator::get_instance ()["new_controller_replaces_player_one"].as<bool> ())
        return (1);

    auto joypads = static_cast <std::vector <std::unique_ptr <joypad>>*> (user_data);

    if (event -> type == SDL_JOYDEVICEADDED)
    {
        LOGGER_INFO ("Controller connected. Replacing player one joypad with it.");

        if ((*joypads)[0] -> get_input_device () == joypad::KEYBOARD)
        {
            (*joypads)[0] -> change_player_number (2);
            (*joypads)[1] -> change_controller_number (SDL_NumJoysticks() - 1);
            (*joypads)[1] -> change_type (joypad::CONTROLLER);
            (*joypads)[1] -> change_player_number (1);
        }

        return (0);
    }

    if (event -> type == SDL_JOYDEVICEREMOVED)
    {
        LOGGER_INFO ("Controller disconnected. Restoring player one controls.");

        (*joypads)[0] -> change_player_number (1);
        joypad::INPUT_DEVICE input_device = configurator::get_instance ()["joypads"][1]["type"].as<std::string>() ==
                                            "keyboard" ? joypad::KEYBOARD : joypad::CONTROLLER;
        (*joypads)[1] -> change_type (input_device);
        (*joypads)[1] -> change_player_number (2);

        return (0);
    }

    return (1);
}


void nes::reload (const std::string &rom_file, bool initialize_controllers)
{
    LOGGER_INFO ("Reloading my_nes.");

    nes_cpu       = std::make_shared<cpu> ();
    cpu_bus       = std::make_shared<bus> (0x0000, 0xFFFF);
    ppu_bus       = std::make_shared<bus> (0x0000, 0x3FFF);
    cpu_ram       = std::make_shared<ram> ();
    nes_cartridge = std::make_shared <cartridge> (rom_file);
    nes_ppu       = std::make_shared <ppu> (this -> game_renderer);

    this -> nes_ppu -> attach (this -> nes_cpu);
    this -> nes_ppu -> attach (this -> nes_cartridge);
    this -> nes_ppu -> set_child_bus (this -> ppu_bus);
    this -> cpu_bus -> add_devices ({this -> nes_ppu, this -> nes_cartridge, this -> cpu_ram, this -> nes_cpu});

    if (initialize_controllers)
        this -> joypads.clear ();

    for (size_t i = 0; i < 2; i++)
    {
        if (initialize_controllers)
            this -> joypads.push_back (std::make_unique <joypad> ());
        this -> cpu_bus -> add_device (this -> joypads[i]);
    }
    if (initialize_controllers)
        this -> load_joypads ();

    this -> rom_loaded    = true;
    this -> emulate_frame = std::bind (&nes::emulate_frame_real, this); // NOLINT

    this -> reset ();
}


void nes::process_events ()
{
    while (SDL_PollEvent (&this -> game_input_event) == 1)
    {
        this -> imgui_manager -> process_event (&this -> game_input_event);
        if (this -> game_input_event.type == SDL_QUIT)
            this -> options.quit = true;
        else if (this -> game_input_event.type == SDL_KEYDOWN && this -> game_input_event.key.keysym.sym == SDLK_p)
            this -> options.show_menu = !this -> options.show_menu;
        else if (this -> game_input_event.type == SDL_DROPFILE)
            reload (game_input_event.drop.file, false);
    }
}


void nes::sleep_until_next_frame (std::chrono::time_point<std::chrono::high_resolution_clock> &frame_start)
{
    static std::chrono::microseconds fps_period;
    static std::chrono::microseconds sleep_time;

    // TODO Solve this mess
    fps_period = std::chrono::microseconds (
            static_cast <int> (static_cast <long long> (( static_cast <double> (std::chrono::microseconds
                    (static_cast <int> ((1000000 / this -> target_fps))).count ()) * (100.0 / this -> options.speed)))));
    sleep_time = std::chrono::duration_cast <std::chrono::microseconds> (fps_period - std::chrono::duration_cast
            <std::chrono::microseconds>(std::chrono::high_resolution_clock::now () - frame_start));

    // Sleep 95% of the remaining time
    std::this_thread::sleep_for (std::chrono::microseconds ((sleep_time.count () / 100) * 95));
    // Block the thread for the remaining 5% of the time
    while (frame_start + fps_period > std::chrono::high_resolution_clock::now ()) {}

    current_fps = 1000000000.0 / static_cast<double> ((std::chrono::high_resolution_clock::now () - frame_start).count ());
    average_fps = (average_fps + current_fps) / 2;
}


void nes::set_title_fps ()
{
    static std::string       title;
    static std::stringstream fps_string_stream;

    fps_string_stream.str (std::string ());
    fps_string_stream << std::fixed << std::setprecision (2) << this -> current_fps;
    title = std::string ("lbnes v" LBNES_VERSION " - FPS: ") + fps_string_stream.str ();

    SDL_SetWindowTitle (this -> game_window, title.c_str ());
}


void nes::populate_palette_2C02()
{
    (this -> color_palette)[0x00] = {84, 84, 84};    (this -> color_palette)[0x01] = {0, 30, 116};
    (this -> color_palette)[0x02] = {8, 16, 144};    (this -> color_palette)[0x03] = {48, 0, 136};
    (this -> color_palette)[0x04] = {68, 0, 100};    (this -> color_palette)[0x05] = {92, 0, 48};
    (this -> color_palette)[0x06] = {84, 4, 0};      (this -> color_palette)[0x07] = {60, 24, 0};
    (this -> color_palette)[0x08] = {32, 42, 0};     (this -> color_palette)[0x09] = {88, 58, 0};
    (this -> color_palette)[0x0A] = {0, 64, 0};      (this -> color_palette)[0x0B] = {0, 60, 0};
    (this -> color_palette)[0x0C] = {0, 50, 60};     (this -> color_palette)[0x0D] = {0, 0, 0};
    (this -> color_palette)[0x0E] = {0, 0, 0};       (this -> color_palette)[0x0F] = {0, 0, 0};

    (this -> color_palette)[0x10] = {152, 150, 152}; (this -> color_palette)[0x11] = {8, 76, 196};
    (this -> color_palette)[0x12] = {48, 50, 236};   (this -> color_palette)[0x13] = {92, 30, 228};
    (this -> color_palette)[0x14] = {136, 20, 176};  (this -> color_palette)[0x15] = {160, 20, 100};
    (this -> color_palette)[0x16] = {152, 34, 32};   (this -> color_palette)[0x17] = {120, 60, 0};
    (this -> color_palette)[0x18] = {84, 90, 0};     (this -> color_palette)[0x19] = {40, 114, 0};
    (this -> color_palette)[0x1A] = {8, 124, 0};     (this -> color_palette)[0x1B] = {0, 118, 40};
    (this -> color_palette)[0x1C] = {0, 102, 120};   (this -> color_palette)[0x1D] = {0, 0, 0};
    (this -> color_palette)[0x1E] = {0, 0, 0};       (this -> color_palette)[0x1F] = {0, 0, 0};

    (this -> color_palette)[0x20] = {236, 238, 236}; (this -> color_palette)[0x21] = {76, 154, 236};
    (this -> color_palette)[0x22] = {120, 124, 236}; (this -> color_palette)[0x23] = {176, 98, 236};
    (this -> color_palette)[0x24] = {228, 84, 236};  (this -> color_palette)[0x25] = {236, 88, 180};
    (this -> color_palette)[0x26] = {236, 106, 100}; (this -> color_palette)[0x27] = {212, 136, 32};
    (this -> color_palette)[0x28] = {160, 170, 0};   (this -> color_palette)[0x29] = {116, 196, 0};
    (this -> color_palette)[0x2A] = {76, 208, 32};   (this -> color_palette)[0x2B] = {56, 204, 108};
    (this -> color_palette)[0x2C] = {56, 180, 204};  (this -> color_palette)[0x2D] = {60, 60, 60};
    (this -> color_palette)[0x2E] = {0, 0, 0};       (this -> color_palette)[0x2F] = {0, 0, 0};

    (this -> color_palette)[0x30] = {236, 238, 236}; (this -> color_palette)[0x31] = {168, 204, 236};
    (this -> color_palette)[0x32] = {188, 188, 236}; (this -> color_palette)[0x33] = {212, 178, 236};
    (this -> color_palette)[0x34] = {236, 174, 236}; (this -> color_palette)[0x35] = {236, 174, 212};
    (this -> color_palette)[0x36] = {236, 180, 176}; (this -> color_palette)[0x37] = {228, 196, 144};
    (this -> color_palette)[0x38] = {204, 210, 120}; (this -> color_palette)[0x39] = {180, 222, 120};
    (this -> color_palette)[0x3A] = {168, 226, 144}; (this -> color_palette)[0x3B] = {152, 226, 180};
    (this -> color_palette)[0x3C] = {160, 214, 228}; (this -> color_palette)[0x3D] = {160, 162, 160};
    (this -> color_palette)[0x3E] = {0, 0, 0};       (this -> color_palette)[0x3F] = {0, 0, 0};
}


void nes::start_server ()
{
    LOGGER_INFO ("Starting server on '" + configurator::get_instance ()["server_ip"].as <std::string> () + ":" \
                     + std::to_string (configurator::get_instance ()["port"].as <short> ()) + "'.");
    this -> options.is_server = true;

    this -> screen_server = std::make_unique <class server> (
            configurator::get_instance ()["server_ip"].as <std::string> ().c_str (),
            configurator::get_instance ()["port"].as <short> ());

    LOGGER_INFO ("Client connected.");
}


void nes::start_client ()
{
    this -> options.is_client = true;

    this -> client_screen_surface = SDL_CreateRGBSurface (0, 256, 240, 24, 0, 0, 0, 0);
    SDL_CreateTextureFromSurface(this->game_renderer, this->client_screen_surface);

    LOGGER_INFO ("Connecting to server on '" + configurator::get_instance ()["server_ip"].as <std::string> () + ":" \
                     + std::to_string (configurator::get_instance ()["port"].as <short> ()) + "'.");

    this -> screen_client = std::make_unique <class client> (
            configurator::get_instance ()["server_ip"].as <std::string> ().c_str (),
            configurator::get_instance ()["port"].as <short> ());

    LOGGER_INFO ("Connected.");
}
