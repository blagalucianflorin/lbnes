//
// Created by lblaga on 18.05.2022.
//

#ifndef NEMULATOR_JOYPAD_H
#define NEMULATOR_JOYPAD_H

#include "devices/device.h"

#include <utility>
#include <vector>
#include <string>
#include <map>

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class joypad : public device
{
public:
    enum INPUT_DEVICE
    {
        KEYBOARD,
        CONTROLLER_ONE,
        CONTROLLER_TWO,
    };

    enum BUTTON
    {
        DPAD_RIGHT = 0,
        DPAD_LEFT = 1,
        DPAD_DOWN = 2,
        DPAD_UP = 3,
        START = 4,
        SELECT = 5,
        A = 6,
        B = 7
    };

private:
    uint8_t      saved_state;
    SDL_Joystick *joystick;
    INPUT_DEVICE input_device;
    bool         activated         = true;
    int          player_number     = 1;

    // TODO implement
    int          controller_number = 1;

    std::vector <std::string> mapping = {"d", "a", "s", "w", "t", "r", "f", "g"};

    std::map <std::string, SDL_Scancode> sdl_scancode_translations =
    {
        {"q", SDL_SCANCODE_Q}, {"w", SDL_SCANCODE_W}, {"e", SDL_SCANCODE_E}, {"r", SDL_SCANCODE_R},
        {"t", SDL_SCANCODE_T}, {"y", SDL_SCANCODE_Y}, {"u", SDL_SCANCODE_U}, {"i", SDL_SCANCODE_I},
        {"o", SDL_SCANCODE_O}, {"p", SDL_SCANCODE_P}, {"a", SDL_SCANCODE_A}, {"s", SDL_SCANCODE_S},
        {"d", SDL_SCANCODE_D}, {"f", SDL_SCANCODE_F}, {"g", SDL_SCANCODE_G}, {"h", SDL_SCANCODE_H},
        {"j", SDL_SCANCODE_J}, {"k", SDL_SCANCODE_K}, {"l", SDL_SCANCODE_L}, {"z", SDL_SCANCODE_Z},
        {"x", SDL_SCANCODE_X}, {"c", SDL_SCANCODE_C}, {"v", SDL_SCANCODE_V}, {"b", SDL_SCANCODE_B},
        {"n", SDL_SCANCODE_N}, {"m", SDL_SCANCODE_M}
    };

public:
    explicit joypad (INPUT_DEVICE input_device = KEYBOARD, int player = 1);

    ~joypad () override = default;

    bool    responds_to (uint16_t address) override;

    void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) override;

    uint8_t read (uint16_t address, bool to_parent_bus = true) override;

    uint8_t set_button (BUTTON button, uint8_t value = 1);

    uint8_t get_button (BUTTON button);

    inline void reset_buttons () { this -> saved_state = 0x00; }

    inline void toggle_activated () { this -> activated = !(this -> activated); }

    inline void set_mapping (std::vector<std::string> new_mapping) { this -> mapping = std::move(new_mapping); }

    inline void change_input_device (INPUT_DEVICE new_input_device) { this -> input_device = new_input_device; }

    inline void change_type (INPUT_DEVICE new_input_device) { this -> input_device = new_input_device; }

    inline void change_player_number (int new_player_number) { this -> player_number = new_player_number; }
};

#endif //NEMULATOR_JOYPAD_H
