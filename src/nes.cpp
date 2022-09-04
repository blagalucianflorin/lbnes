//
// Created by lblaga on 27.05.2022.
//

#include <memory>

#include "nes.h"

nes::nes (const std::string& rom_file) :
    nes_cpu (),
    cpu_bus (0x0000, 0xFFFF),
    ppu_bus (0x0000, 0x3FFF),
    cpu_ram (),
    palette_ram (),
    nes_cartridge (rom_file)
{
    main_window (this -> game_window,  this -> game_renderer, "lbnes", WINDOW_WIDTH, WINDOW_HEIGHT);

    nes_ppu = std::make_unique<ppu> (this -> game_renderer);

    std::ifstream fin ("config.yaml");
    if (fin.good ())
    {
        fin.close ();
        this -> config = YAML::LoadFile ("config.yaml");
    }

    for (int i = 0; i < 2; i++)
    {
        this -> joypads.push_back (std::make_unique<joypad>());
        this -> cpu_bus.add_device (this -> joypads[i].get ());
    }

    this -> nes_ppu -> attach (&(this -> nes_cpu));
    this -> nes_ppu -> attach (&(this -> nes_cartridge));
    this -> nes_ppu -> set_child_bus (this -> ppu_bus);
    this -> ppu_bus.add_devices ({&(this -> palette_ram)});
    this -> cpu_bus.add_devices ({&(this -> nes_cpu), &(this -> cpu_ram), &(this -> nes_cartridge),
                                  (this -> nes_ppu).get ()});

    this -> load_joypads ();

    this -> reset ();
}

nes::~nes ()
{
    SDL_DestroyRenderer (this -> game_renderer);
    SDL_DestroyWindow (this -> game_window);
    SDL_Quit ();
}

void nes::start ()
{
    this -> main_loop ();
}

void nes::reset ()
{
    this -> nes_cpu.reset ();
    this -> nes_cartridge.reset ();
    this -> nes_ppu -> reset ();
}

void nes::main_loop ()
{
    double                    fps_period  = 1000000.0 / this -> target_fps;
    double                    average_fps = 60.098814;
    bool                      quit        = false;
    auto                      frame_start = std::chrono::high_resolution_clock::now ();
    auto                      last_time   = std::chrono::high_resolution_clock::now ();
    std::chrono::microseconds delta_time;

    while (!quit)
    {
        frame_start = std::chrono::high_resolution_clock::now ();
        while (SDL_PollEvent (&this -> game_input_event) == 1)
            if (this -> game_input_event.type == SDL_QUIT)
                quit = true;
            else if (this -> game_input_event.type == SDL_KEYDOWN && this -> game_input_event.key.keysym.sym == SDLK_p)
                    ToggleFullscreen (this -> game_window);

        delta_time  = std::chrono::duration_cast<std::chrono::microseconds> (frame_start - last_time);
        if ((double) delta_time.count () >= fps_period)
        {
            last_time   = frame_start;
            average_fps = (average_fps + (1000000.00 / (double) delta_time.count ())) / 2;

            this -> render_frame ();

            SDL_SetWindowTitle (this -> game_window,
                                ("FPS: " + std::to_string (1000000.00 / (double) delta_time.count ())).c_str ());
        }
    }
    std::cout << "Average FPS: " << average_fps << std::endl;
}

uint32_t *nes::render_frame ()
{
    for (int i = 0; i < (262 * 341) + (this -> nes_ppu -> is_odd_frame () ? 1 : 0); i++)
    {
        this -> nes_ppu -> clock ();
        if (this -> total_cycles % 3 == 0)
            (this -> nes_cpu).clock ();

        (this -> total_cycles)++;
    }

    SDL_RenderPresent (this -> game_renderer);

    return (this -> nes_ppu -> get_pixels ());
}

uint8_t nes::get_button (joypad::BUTTON button, uint8_t player)
{
    return ((this -> joypads)[player] -> get_button (button));
}

uint8_t nes::set_button (joypad::BUTTON button, uint8_t value, uint8_t player)
{
    return ((this -> joypads)[player] -> set_button (button, value));
}

void nes::reset_buttons (uint8_t player)
{
    (this -> joypads)[player] -> reset_buttons ();
}

void nes::toggle_joypad (uint8_t player)
{
    (this -> joypads)[player] -> toggle_activated ();
}

void nes::load_joypads ()
{
    if (!config["joypads"])
    {
        (this -> joypads)[0] = std::make_unique<joypad> (joypad::KEYBOARD, 1);
        (this -> joypads)[1] = std::make_unique<joypad> (joypad::CONTROLLER_ONE, 2);
        this -> cpu_bus.add_device ((this -> joypads)[0].get ());
        this -> cpu_bus.add_device ((this -> joypads)[1].get ());

        return;
    }

    int current_player = 1;
    auto players = config["joypads"];
    for (auto player : players)
    {
        if (player["type"].as<std::string>() == "keyboard")
        {
            std::vector<std::string> player_one_mapping;
            player_one_mapping.push_back (player["mapping"]["dpad-right"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["dpad-left"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["dpad-up"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["dpad-down"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["start"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["select"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["a"].as<std::string>());
            player_one_mapping.push_back (player["mapping"]["b"].as<std::string>());

            (this -> joypads)[current_player - 1] -> change_type (joypad::KEYBOARD);
            (this -> joypads)[current_player - 1] -> set_mapping (player_one_mapping);
            (this -> joypads)[current_player - 1] -> change_player_number (current_player);
        }
        else
        {
            (this -> joypads)[current_player - 1] -> change_type (joypad::CONTROLLER_ONE);
            (this -> joypads)[current_player - 1] -> change_player_number (current_player);
        }
        current_player++;
    }
}