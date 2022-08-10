//
// Created by lblaga on 18.05.2022.
//

#include "include/devices/inputs/joypad.h"

#include <iostream>

joypad::joypad (INPUT_DEVICE input_device, int player) : device (0x4016, 0x4017)
{
    this -> saved_state   = 0x00;
    this -> player_number = player;
    this -> input_device  = input_device;

    if (SDL_NumJoysticks () > 0 && (this -> input_device == CONTROLLER_ONE || this -> input_device == CONTROLLER_TWO))
    {
        SDL_JoystickEventState(SDL_ENABLE);
        this -> joystick = SDL_JoystickOpen(this -> input_device == CONTROLLER_ONE ? 0 : 1);
    }
}

bool joypad::responds_to (uint16_t address)
{
    return ((this -> player_number == 1 && address == 0x4016) || (this -> player_number == 2 && address == 0x4017));
}

void joypad::write (uint16_t address, uint8_t data, bool to_parent_bus)
{
    if (!activated)
        return;

    this -> saved_state = 0x00;
    const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);

    if (this -> input_device == KEYBOARD_ONE)
    {
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_A] ? 1 : 0) << 1;  // D-LEFT
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_D] ? 1 : 0) << 0;  // D-RIGHT
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_W] ? 1 : 0) << 3;  // D-UP
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_S] ? 1 : 0) << 2;  // D-DOWN
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_F] ? 1 : 0) << 6;  // A
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_G] ? 1 : 0) << 7;  // B
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_T] ? 1 : 0) << 4;  // Start
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_R] ? 1 : 0) << 5;  // Select
    }

    if (this -> input_device == KEYBOARD_TWO)
    {
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_LEFT] ? 1 : 0) << 1;  // D-LEFT
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_RIGHT] ? 1 : 0) << 0; // D-RIGHT
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_UP] ? 1 : 0) << 3;    // D-UP
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_DOWN] ? 1 : 0) << 2;  // D-DOWN
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_N] ? 1 : 0) << 6;     // A
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_M] ? 1 : 0) << 7;     // B
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_K] ? 1 : 0) << 4;     // Start
        this -> saved_state |= (keyboard_state[SDL_SCANCODE_J] ? 1 : 0) << 5;     // Select
    }

    if (this -> input_device == CONTROLLER_ONE || this -> input_device == CONTROLLER_TWO)
    {
        if (SDL_NumJoysticks () < 1)
            return;

        this -> saved_state |= (SDL_JoystickGetHat (this -> joystick, 0) == SDL_HAT_LEFT ? 1 : 0) << 1;
        this -> saved_state |= (SDL_JoystickGetHat (this -> joystick, 0) == SDL_HAT_RIGHT ? 1 : 0) << 0;
        this -> saved_state |= (SDL_JoystickGetHat (this -> joystick, 0) == SDL_HAT_UP ? 1 : 0) << 3;
        this -> saved_state |= (SDL_JoystickGetHat (this -> joystick, 0) == SDL_HAT_DOWN ? 1 : 0) << 2;
        this -> saved_state |= (SDL_JoystickGetButton (this -> joystick, 0) ? 1 : 0) << 7;
        this -> saved_state |= (SDL_JoystickGetButton (this -> joystick, 1) ? 1 : 0) << 6;
        this -> saved_state |= (SDL_JoystickGetButton (this -> joystick, 7) ? 1 : 0) << 4;
        this -> saved_state |= (SDL_JoystickGetButton (this -> joystick, 6) ? 1 : 0) << 5;
    }
}

uint8_t joypad::read (uint16_t address, bool to_parent_bus)
{
    uint8_t result = (this -> saved_state & 0b10000000) >> 7;

    this -> saved_state <<= 1;

    return (result);
}

uint8_t joypad::set_button (joypad::BUTTON button, uint8_t value)
{
    value = value ? 1 : 0;

    if (this -> get_button (button) != value)
        this -> saved_state ^= 1 << button;

    return (this -> saved_state);
}

uint8_t joypad::get_button (joypad::BUTTON button)
{
    return (((this -> saved_state) >> button) & 1);
}
