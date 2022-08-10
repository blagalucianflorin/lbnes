//
// Created by lblaga on 12.03.2021.
//

#include "debugging/nes_debugger.h"

void code_injector (const char *file_name, uint16_t start_address, cpu *my_cpu)
{
    std::ifstream fin (file_name);
    std::string   curr_word;
    uint16_t      curr_address    = start_address;

    while (fin >> curr_word)
        if (curr_word.length () == 2)
        {
            my_cpu -> write (curr_address, std::stoi (curr_word, nullptr, 16));
            curr_address++;
        }

    fin.close ();
}

void print_hex (int x, int width, char padding = '0', bool right = true)
{
    std::cout << std::setfill (padding);
    std::cout << std::setw (width);
    std::cout << (right ? std::right : std::left);
    std::cout << std::hex << x;
    std::cout << std::dec;
}

void print_memory (cpu *my_cpu, uint16_t start, uint16_t lines)
{
    std::cout << "\t";
    for (int i = 0; i < 16; i++)
        print_hex (i, 2), std::cout << " ";
    std::cout << std::endl;
    for (int i = 0; i < lines; i++)
    {
        print_hex ((int) start + (i * 16), 4), std::cout << "\t";
        for (int j = 0; j < 16; j++)
        {
            if (start + (i * 16) + j == my_cpu -> program_counter)
                std::cout << "\033[1;4;37;40m";
            print_hex((int) my_cpu->read(start + (i * 16) + j), 2);
            if (start + (i * 16) + j == my_cpu -> program_counter)
                std::cout << "\033[0m";
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

/* SDL2 printing */

SDL_Color c_white = {255, 255, 255, 255};
SDL_Color c_black = {0, 0, 0, 255};
SDL_Color c_red   = {255, 0, 0, 255};
SDL_Color c_green = {0, 255, 0, 255};
SDL_Color c_blue  = {0, 0, 255, 255};

t_coords render_text (t_cpu_drawer cpu_drawer, const char *text, int x, int y, SDL_Color color)
{
    SDL_Renderer *renderer  = cpu_drawer.renderer;
    TTF_Font     *font      = cpu_drawer.font;
    SDL_Texture  *texture;
    SDL_Rect     rect;
    SDL_Surface  *surface;
    auto         ret_coords = new t_coords;

    surface = TTF_RenderText_Blended (font, text, color);
    texture = SDL_CreateTextureFromSurface (renderer, surface);

    rect.x      = x;
    rect.y      = y;
    rect.w      = surface -> w;
    rect.h      = surface -> h;
    *ret_coords = {x, y, rect.w, rect.h};

    SDL_FreeSurface (surface);
    SDL_RenderCopy (renderer, texture, nullptr, &rect);
    SDL_DestroyTexture (texture);

    return (*ret_coords);
}

std::string to_hexa (int number, int width, bool hex_prefix)
{
    std::stringstream   sstream;
    auto                output  = new std::string;

    if (hex_prefix)
        sstream << "0x";
    sstream << std::setfill ('0');
    sstream << std::uppercase;
    sstream << std::hex;
    sstream << std::setw (width);
    sstream << number;
    *output = sstream.str ();

    return (*output);
}

t_coords render_flags (t_cpu_drawer cpu_drawer, int x, int y)
{
    auto        last_coords = new t_coords;
    cpu         *my_cpu     = cpu_drawer.my_cpu;
    std::string texts[8];

    texts[0] = 'C', texts[1] = 'Z', texts[2] = 'I', texts[3] = 'D';
    texts[4] = 'B', texts[5] = 'U', texts[6] = 'O', texts[7] = 'N';

    *last_coords = render_text (cpu_drawer, "FLAGS: ", x, y, c_black);
    for (int i = 0; i < 8; i++)
    {
        *last_coords = render_text (cpu_drawer, texts[i].c_str (),last_coords -> x + last_coords -> w + 5, y,
                                    (my_cpu -> get_flag ((cpu::FLAG) i) ? c_green : c_red));
    }

    last_coords -> x = x;
    last_coords -> y = y;
    return (*last_coords);
}

t_coords render_program_counter (t_cpu_drawer cpu_drawer, int x, int y)
{
    auto              last_coords = new t_coords;
    cpu               *my_cpu     = cpu_drawer.my_cpu;
    std::stringstream sstream;
    std::string       text;

    sstream << std::setfill ('0');
    sstream << std::uppercase;
    sstream << std::hex;
    sstream << std::setw (4);
    sstream << (int) my_cpu -> program_counter;
    text = "PROGRAM COUNTER: 0x" + sstream.str();

    *last_coords     = render_text (cpu_drawer, text.c_str (), x, y, c_black);
    last_coords -> x = x;
    last_coords -> y = y;

    return (*last_coords);
}

t_coords render_cycles_elapsed (t_cpu_drawer cpu_drawer, int x, int y)
{
    auto              last_coords = new t_coords;
    cpu               *my_cpu     = cpu_drawer.my_cpu;
    std::stringstream sstream;
    std::string       text;

    sstream << my_cpu -> cycles_elapsed;
    text             = "CYCLES ELAPSED: " + sstream.str ();
    *last_coords     = render_text (cpu_drawer, text.c_str (), x, y, c_black);
    last_coords -> x = x;
    last_coords -> y = y;

    return (*last_coords);
}

t_coords render_cpu (t_cpu_drawer cpu_drawer, int x, int y)
{
    auto         last_coords     = new t_coords;
//    SDL_Rect     background_rect;

    *last_coords = render_text (cpu_drawer, "CPU", x, y, c_black);
    *last_coords = render_flags (cpu_drawer, x,(last_coords -> y + last_coords -> h));
    *last_coords = render_program_counter (cpu_drawer, x,(last_coords -> y + last_coords -> h));
    *last_coords = render_cycles_elapsed (cpu_drawer, x,(last_coords -> y + last_coords -> h));

    return (*last_coords);
}


t_coords render_memory (t_cpu_drawer cpu_drawer, int x, int y, uint16_t start_address, uint16_t lines,
                        bool pc_highlight)
{
    auto        last_coords  = new t_coords;
    cpu         *my_cpu      = cpu_drawer.my_cpu;
    std::string line_address;
    std::string cell_value;
    SDL_Color   aux_color;
    int         aux;

    last_coords -> h = 0;

    for (int i = 0; i < lines; i++)
    {
        line_address = to_hexa (start_address + (i * 16), 4, true);
        line_address += ": ";
        *last_coords = render_text (cpu_drawer, line_address.c_str (), x, y + last_coords -> h, c_black);

        for (int j = 0; j < 16; j++)
        {
            aux          = my_cpu  -> read (start_address + (i * 16) + j);
            cell_value   = to_hexa (aux, 2, false);
            cell_value   += " ";
            aux_color    = start_address + (i * 16) + j == my_cpu -> program_counter ? c_blue : c_black;
            *last_coords = render_text (cpu_drawer, cell_value.c_str (),last_coords -> x + last_coords -> w,
                                        last_coords -> y, aux_color);
        }
    }

    last_coords -> x = x;
    last_coords -> y = y;

    return (*last_coords);
}

void print_state (t_cpu_drawer cpu_drawer, SDL_Renderer *renderer, ppu *nes_ppu)
{
    t_coords *last_coords = new t_coords;
    nes_ppu -> draw_nametable ();

    draw_tiles (nes_ppu);
    draw_palette (renderer, nes_ppu);
    draw_whole_palette (renderer, nes_ppu);

    *last_coords = render_cpu (cpu_drawer, 15, 15);

    *last_coords = render_text (cpu_drawer, "ZERO PAGE:", 16, last_coords -> y + last_coords -> h + 50,
                                {0, 0, 0, 0});
    *last_coords = render_memory (cpu_drawer, 15,last_coords -> y + last_coords -> h,0x0000, 2, true);

    *last_coords = render_text (cpu_drawer, "STACK:", 16,last_coords -> y + last_coords -> h + 50,{0, 0, 0, 0});
    *last_coords = render_memory (cpu_drawer, 15,last_coords -> y + last_coords -> h,0x01E0, 2, false);

    *last_coords = render_text (cpu_drawer, "PROGRAM COUNTER MEMORY:", 16,last_coords -> y + last_coords -> h + 50,{0, 0, 0, 0});
    *last_coords = render_memory (cpu_drawer, 15,last_coords -> y + last_coords -> h,cpu_drawer.my_cpu -> program_counter & 0xFFF0, 2, true);
}