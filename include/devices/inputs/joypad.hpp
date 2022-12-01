//
// Created by lblaga on 18.05.2022.
//

#ifndef JOYPAD_HPP
#define JOYPAD_HPP

#include <utility>
#include <vector>
#include <string>
#include <map>

#include <SDL.h>

#include "devices/device.hpp"
#include "network/client.hpp"
#include "network/server.hpp"


class joypad : public device
{
public:
    enum INPUT_DEVICE
    {
        KEYBOARD,
        CONTROLLER
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
    uint8_t             saved_state;
    SDL_GameController *controller;
    INPUT_DEVICE        input_device;
    bool                activated           = true;
    int                 player_number       = 1;
    int                 controller_number   = 0;
    int                 controller_deadzone = 5000;

    std::vector <std::string> mapping {"d", "a", "s", "w", "t", "r", "f", "g"};

    std::map <std::string, SDL_Scancode> sdl_scancode_translations =
    {
        {"q", SDL_SCANCODE_Q}, {"w", SDL_SCANCODE_W}, {"e", SDL_SCANCODE_E}, {"r", SDL_SCANCODE_R},
        {"t", SDL_SCANCODE_T}, {"y", SDL_SCANCODE_Y}, {"u", SDL_SCANCODE_U}, {"i", SDL_SCANCODE_I},
        {"o", SDL_SCANCODE_O}, {"p", SDL_SCANCODE_P}, {"a", SDL_SCANCODE_A}, {"s", SDL_SCANCODE_S},
        {"d", SDL_SCANCODE_D}, {"f", SDL_SCANCODE_F}, {"g", SDL_SCANCODE_G}, {"h", SDL_SCANCODE_H},
        {"j", SDL_SCANCODE_J}, {"k", SDL_SCANCODE_K}, {"l", SDL_SCANCODE_L}, {"z", SDL_SCANCODE_Z},
        {"x", SDL_SCANCODE_X}, {"c", SDL_SCANCODE_C}, {"v", SDL_SCANCODE_V}, {"b", SDL_SCANCODE_B},
        {"n", SDL_SCANCODE_N}, {"m", SDL_SCANCODE_M},

        {"arrows-up", SDL_SCANCODE_UP}, {"arrows-down", SDL_SCANCODE_DOWN},
        {"arrows-left", SDL_SCANCODE_LEFT}, {"arrows-right", SDL_SCANCODE_RIGHT},

        {"keypad-1", SDL_SCANCODE_KP_1}, {"keypad-2", SDL_SCANCODE_KP_2}, {"keypad-3", SDL_SCANCODE_KP_3},
        {"keypad-4", SDL_SCANCODE_KP_4}, {"keypad-5", SDL_SCANCODE_KP_5}, {"keypad-6", SDL_SCANCODE_KP_6},
        {"keypad-7", SDL_SCANCODE_KP_7}, {"keypad-8", SDL_SCANCODE_KP_8}, {"keypad-9", SDL_SCANCODE_KP_9},
    };

public:
    explicit joypad (INPUT_DEVICE input_device = KEYBOARD, int player = 1);

    ~joypad () override = default;

    bool    responds_to (uint16_t address) override;

    void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t read (uint16_t address, bool to_parent_bus = true) override; // NOLINT

    uint8_t set_button (BUTTON button, uint8_t value = 1);

    [[nodiscard]] uint8_t get_button (BUTTON button) const;

    void    change_type (INPUT_DEVICE new_input_device);

    inline void reset_buttons () { this -> saved_state = 0x00; }

    inline void toggle_activated () { this -> activated = !(this -> activated); }

    inline void set_mapping (std::vector<std::string> new_mapping) { this -> mapping = std::move(new_mapping); }

    inline void change_input_device (INPUT_DEVICE new_input_device) { this -> input_device = new_input_device; }

    inline INPUT_DEVICE get_input_device () { return (this -> input_device); }

    inline void change_player_number (int new_player_number) { this -> player_number = new_player_number; }

    inline void change_controller_number (int new_controller_number) { this -> controller_number = new_controller_number; }

    inline void                  set_state (const uint8_t new_state) { this -> saved_state = new_state; }

    [[nodiscard]] inline uint8_t get_state () const { return (this -> saved_state); }
};

#endif //JOYPAD_HPP
