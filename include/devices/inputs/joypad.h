//
// Created by lblaga on 18.05.2022.
//

#ifndef NEMULATOR_JOYPAD_H
#define NEMULATOR_JOYPAD_H

#include "include/devices/device.h"

#include <SDL2/SDL.h>

class joypad : public device
{
public:
    enum INPUT_DEVICE
    {
        KEYBOARD_ONE,
        KEYBOARD_TWO,
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
    int          player_number = 1;
    INPUT_DEVICE input_device;
    bool         activated = true;

public:
    explicit joypad (INPUT_DEVICE input_device = KEYBOARD_ONE, int player = 1);

    ~joypad () override = default;

    bool    responds_to (uint16_t address) override;

    void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) override;

    uint8_t read (uint16_t address, bool to_parent_bus = true) override;

    inline void set_input_device (INPUT_DEVICE input_device) { this -> input_device = input_device; }

    uint8_t set_button (BUTTON button, uint8_t value = 1);

    uint8_t get_button (BUTTON button);

    inline void reset_buttons () { this -> saved_state = 0x00; }

    inline void toggle_activated () { this -> activated = !(this -> activated); }
};

#endif //NEMULATOR_JOYPAD_H
