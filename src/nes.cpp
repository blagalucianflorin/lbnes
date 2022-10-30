//
// Created by lblaga on 27.05.2022.
//

#include "nes.h"


nes::nes ()
{
    auto rom_file = configurator::get_instance ()["rom"].as <std::string> ();

    create_sdl_window (this -> game_window, this -> game_renderer);
    SDL_SetEventFilter (nes::controller_connection_event_manager, &(this -> joypads));

    this -> imgui_manager = std::make_shared <class imgui_manager> (this -> game_renderer, this -> game_window, this);

    this -> options.display_fps = configurator::get_instance ()["display_fps"].as <bool> ();
    this -> options.fullscreen  = configurator::get_instance ()["fullscreen"].as <bool> ();
    this -> options.show_menu   = configurator::get_instance ()["show_menu"].as <bool> ();
    this -> options.vsync       = configurator::get_instance ()["vsync"].as <bool> ();
    this -> options.speed       = configurator::get_instance ()["speed"].as <int> ();
    this -> options.quit        = false;

    if (configurator::get_instance ()["server"].as <bool> ())
    {
        this -> is_server = true;

        std::cout << "Starting server on " << configurator::get_instance ()["server_ip"].as <std::string> ()
                << ":" << configurator::get_instance ()["port"].as <short> ()
                << ". Waiting for connections..." << std::endl;

        this -> screen_server = std::make_unique <class server> (
                configurator::get_instance ()["server_ip"].as <std::string> ().c_str (),
                configurator::get_instance ()["port"].as <short> ());

        std::cout << "Client connected!";
    }

    if (configurator::get_instance ()["client"].as <bool> ())
    {
        this -> is_client = true;

        this -> client_screen_surface = SDL_CreateRGBSurface (0, 256, 240, 24, 0, 0, 0, 0);
        this -> client_screen_texture = SDL_CreateTextureFromSurface (this -> game_renderer, this -> client_screen_surface);

        std::cout << "Connecting to server on " << configurator::get_instance ()["server_ip"].as <std::string> ()
                  << ":" << configurator::get_instance ()["port"].as <short> () << "..." << std::endl;

        this -> screen_client = std::make_unique <class client> (
                configurator::get_instance ()["server_ip"].as <std::string> ().c_str (),
                configurator::get_instance ()["port"].as <short> ());

        std::cout << "Connected!";
    }

    if (!rom_file.empty ())
        this -> reload (rom_file, true);
}


nes::~nes ()
{
    this -> imgui_manager.reset ();

    SDL_DestroyRenderer (this -> game_renderer);
    SDL_DestroyWindow (this -> game_window);
    SDL_Quit ();
}


void nes::start ()
{
    auto frame_start = std::chrono::high_resolution_clock::now ();

    while (!this -> options.quit)
    {
        frame_start = std::chrono::high_resolution_clock::now();

        this -> process_events ();
        this -> render_frame ();
        if (!this -> is_client)
            this -> sleep_until_next_frame (frame_start);
        this -> set_title ();
    }
}


void nes::reset ()
{
    if (!this -> rom_loaded)
        return;

    this -> nes_cpu -> reset ();
    this -> nes_cartridge -> reset ();
    this -> nes_ppu -> reset ();
}


uint32_t *nes::render_frame ()
{
    SDL_SetRenderDrawColor (this -> game_renderer, 128, 128, 128, 255);
    SDL_RenderClear (this -> game_renderer);

    if (this -> is_client)
    {
        this -> screen_client -> receive_screen (this -> client_pixels);

        for (int i = 0; i < 240; i++)
            for (int j = 0; j < 256; j++)
                surface_set_pixel (this -> client_screen_surface, j, i, this -> client_pixels[i * 240 + j]);

        SDL_DestroyTexture (this -> client_screen_texture);
        this -> client_screen_texture = SDL_CreateTextureFromSurface (this -> game_renderer, this -> client_screen_surface);
        SDL_RenderCopy (this -> game_renderer, this -> client_screen_texture, nullptr, nullptr);
    }
    else
    {
        if (this -> rom_loaded)
        {
            for (int i = 0; i < (262 * 341) + (this -> nes_ppu -> is_odd_frame () ? 1 : 0); i++)
            {
                this -> nes_ppu -> clock ();
                if (this -> total_cycles % 3 == 0)
                    (this -> nes_cpu) -> clock ();

                (this -> total_cycles)++;
            }
        }
    }

    this -> imgui_manager -> draw_menu ();

    if (this -> is_server)
        this -> screen_server -> send_screen (this -> nes_ppu -> get_pixels ());

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
}


[[maybe_unused]] void nes::toggle_joypad (uint8_t player)
{
    (this -> joypads)[player] -> toggle_activated ();
}


void nes::load_joypads ()
{
    if (!configurator::get_instance ()["joypads"])
    {
        (this -> joypads)[0] = std::make_unique<joypad> (joypad::KEYBOARD, 1);
        (this -> joypads)[1] = std::make_unique<joypad> (joypad::CONTROLLER, 2);
        this -> cpu_bus -> add_device ((this -> joypads)[0].get ());
        this -> cpu_bus -> add_device ((this -> joypads)[1].get ());

        return;
    }

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

            (this -> joypads)[player["player"].as<int>() - 1] -> change_type (joypad::KEYBOARD);
            (this -> joypads)[player["player"].as<int>() - 1] -> set_mapping (player_one_mapping);
            (this -> joypads)[player["player"].as<int>() - 1] ->
                    change_player_number (player["player"].as<int>());
        }
        else
        {
            (this -> joypads)[player["player"].as<int>() - 1] ->
                    change_controller_number (player["controller_index"].as<int>() - 1);
            (this -> joypads)[player["player"].as<int>() - 1] -> change_type (joypad::CONTROLLER);
            (this -> joypads)[player["player"].as<int>() - 1] ->
                    change_player_number (player["player"].as<int>());
        }
    }
}


int nes::controller_connection_event_manager (void *user_data, SDL_Event *event)
{
    if (!configurator::get_instance ()["new_controller_replaces_player_one"].as<bool> ())
        return (1);

    auto joypads = (std::vector <std::unique_ptr <joypad>>*) user_data;

    if (event -> type == SDL_JOYDEVICEADDED)
    {
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
    nes_cpu       = std::make_shared<cpu> ();
    cpu_bus       = std::make_shared<bus> (0x0000, 0xFFFF);
    ppu_bus       = std::make_shared<bus> (0x0000, 0x3FFF);
    cpu_ram       = std::make_shared<ram> ();
    nes_cartridge = std::make_shared <cartridge> (rom_file);
    nes_ppu       = std::make_shared <ppu> (this -> game_renderer);

    if (initialize_controllers)
        this -> joypads.clear ();

    for (int i = 0; i < 2; i++)
    {
        if (initialize_controllers)
            this -> joypads.push_back (std::make_unique <joypad> ());
        this -> cpu_bus -> add_device (this -> joypads[i].get ());
    }
    if (initialize_controllers)
        this -> load_joypads ();

    this -> nes_ppu -> attach (this -> nes_cpu.get ());
    this -> nes_ppu -> attach (this -> nes_cartridge.get ());
    this -> nes_ppu -> set_child_bus (*(this -> ppu_bus));
    this -> cpu_bus -> add_devices ({this -> nes_cpu.get (), this -> cpu_ram.get (), this -> nes_cartridge.get (),
                                     (this -> nes_ppu).get ()});

    this -> rom_loaded = true;

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

    fps_period = std::chrono::microseconds ((int) ((float) (std::chrono::microseconds
            ((int) (1000000 / this -> target_fps))).count () * (100.0 / this -> options.speed)));
    sleep_time = std::chrono::duration_cast <std::chrono::microseconds> (fps_period - std::chrono::duration_cast
            <std::chrono::microseconds>(std::chrono::high_resolution_clock::now () - frame_start));

    // Sleep 95% of the remaining time
    std::this_thread::sleep_for (std::chrono::microseconds ((sleep_time.count () / 100) * 95));
    // Block the thread for the remaining 5% of the time
    while (frame_start + fps_period > std::chrono::high_resolution_clock::now ()) {}

    current_fps = 1000000000.0 / (double) (std::chrono::high_resolution_clock::now () - frame_start).count ();
    average_fps = (average_fps + current_fps) / 2;
}


void nes::set_title ()
{
    static std::string title;
    static std::stringstream fps_string_stream;

    if (this -> options.display_fps)
    {
        fps_string_stream.str (std::string ());
        fps_string_stream << std::fixed << std::setprecision (2) << this -> current_fps;
        title = std::string ("lbnes v" LBNES_VERSION " - FPS: ") + fps_string_stream.str ();
    }
    else
        title = "lbnes v" LBNES_VERSION;

    SDL_SetWindowTitle (this -> game_window, title.c_str ());
}
