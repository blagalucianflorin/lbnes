//
// Created by lblaga on 27.05.2022.
//

#include "include/nes.h"

nes::nes (std::string rom_file)
{
    main_window (game_window, game_renderer, "nemulator", WINDOW_WIDTH, WINDOW_HEIGHT);

    this -> cpu_bus           = new bus (0x0000, 0xFFFF);
    this -> nes_cpu           = new cpu ();
    this -> player_one_joypad = new joypad (joypad::KEYBOARD_ONE, 1);
    this -> player_two_joypad = new joypad (joypad::KEYBOARD_TWO, 2);
    this -> nes_ppu           = new ppu (this -> game_renderer);
    this -> cpu_ram           = new ram ();
    this -> palette_ram       = new ppu_palette_ram ();
    this -> ppu_bus           = new bus (0x0000, 0x3FFF);
    this -> nes_cartridge     = new cartridge (rom_file);

    this -> nes_ppu -> attach (this -> nes_cpu);
    this -> nes_ppu -> attach (this -> nes_cartridge);
    this -> cpu_bus -> add_devices ({this -> nes_cpu, this -> cpu_ram, this -> nes_cartridge, this -> nes_ppu,
                                     this -> player_one_joypad, this -> player_two_joypad});
    this -> ppu_bus -> add_device (this -> palette_ram);
    this -> nes_ppu -> set_child_bus (*(this -> ppu_bus));

    this -> reset ();
}

nes::~nes ()
{
    delete (this -> ppu_bus);
    delete (this -> cpu_bus);

    SDL_DestroyRenderer (this -> game_renderer);
    SDL_DestroyWindow (this -> game_window);
    SDL_Quit ();
}

void nes::start ()
{
    this -> running = true;

    if (!(this -> main_loop_started))
        this -> main_loop ();
}

void nes::reset ()
{
    if (this -> nes_cpu == nullptr || this -> nes_ppu == nullptr || this -> nes_cartridge == nullptr)
        return;

    this -> nes_cpu -> reset ();
    this -> nes_ppu -> reset ();
    this -> nes_cartridge -> reset ();
}

void nes::main_loop ()
{
    if (this -> nes_cpu == nullptr || this -> nes_ppu == nullptr || this -> nes_cartridge == nullptr)
        throw std::exception ();

    this -> main_loop_started = true;

    double fps_period  = 1000000.0 / this -> target_fps;
    double average_fps = 60.098814;

    auto frame_start = std::chrono::high_resolution_clock::now ();
    auto last_time   = std::chrono::high_resolution_clock::now ();
    std::chrono::microseconds delta_time;

    bool quit = false;
    int  i;

    while (!quit)
    {
        while (SDL_PollEvent(&this -> game_input_event) == 1)
            if (this -> game_input_event.type == SDL_QUIT)
                quit = true;
            else if(this -> game_input_event.type == SDL_KEYDOWN)
            {
                if (this -> game_input_event.key.keysym.sym == SDLK_o)
                    this -> running = !(this -> running);
                else if (this -> game_input_event.key.keysym.sym == SDLK_p)
                    ToggleFullscreen (this -> game_window);
            }

        frame_start = std::chrono::high_resolution_clock::now ();
        delta_time  = std::chrono::duration_cast<std::chrono::microseconds> (frame_start - last_time);

        if ((double) delta_time.count () >= fps_period)
        {
            last_time   = frame_start;
            average_fps = (average_fps + (1000000.00 / delta_time.count ())) / 2;

            SDL_SetWindowTitle (this -> game_window, ("FPS: " + std::to_string(1000000.00 / delta_time.count ())).c_str ());

            if (this -> running)
            {
                for (i = 0; i < 262 * 341; i++)
                {
                    this -> nes_ppu -> clock ();
                    if (this -> total_cycles % 3 == 0)
                        this -> nes_cpu -> clock ();

                    this -> total_cycles++;
                }
            }

            SDL_RenderPresent (this -> game_renderer);
        }
    }
    std::cout << "Average FPS: " << average_fps << std::endl;
}

uint32_t *nes::render_frame ()
{
    static int i;

    for (i = 0; i < 262 * 341; i++)
    {
        this -> nes_ppu -> clock ();
        if (this -> total_cycles % 3 == 0)
            this -> nes_cpu -> clock ();

        this -> total_cycles++;
    }

    SDL_RenderPresent (this -> game_renderer);

    return (this -> nes_ppu -> get_pixels ());
}

uint8_t nes::get_button (joypad::BUTTON button, uint8_t player)
{
    return ((player == 1 ? this -> player_one_joypad : this -> player_two_joypad) -> get_button (button));
}

uint8_t nes::set_button (joypad::BUTTON button, uint8_t value, uint8_t player)
{
    return ((player == 1 ? this -> player_one_joypad : this -> player_two_joypad) -> set_button (button, value));
}

void nes::reset_buttons (uint8_t player)
{
    (player == 1 ? this -> player_one_joypad : this -> player_two_joypad) -> reset_buttons ();
}

void nes::toggle_joypad (uint8_t player)
{
    (player == 1 ? this -> player_one_joypad : this -> player_two_joypad) -> toggle_activated ();
}