//
// Created by lblaga on 18.05.2022.
//

#include "devices/inputs/joypad.hpp"

#include <iostream>

joypad::joypad (INPUT_DEVICE input_device, int player) : device (0x4016, 0x4017)
{
    this -> saved_state   = 0x00;
    this -> player_number = player;
    this -> input_device  = input_device;

    if (SDL_NumJoysticks () > 0 && (this -> input_device == CONTROLLER))
    {
        SDL_JoystickEventState(SDL_ENABLE);
        this -> controller = SDL_GameControllerOpen (this -> controller_number);
    }
}

bool joypad::responds_to (uint16_t address)
{
    return ((this -> player_number == 1 && address == 0x4016) || (this -> player_number == 2 && address == 0x4017));
}

void joypad::write (uint16_t /*address*/, uint8_t /*data*/, bool /*to_parent_bus*/) // NOLINT
{
    if (!activated)
        return;

    this -> saved_state = 0x00;
    const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);

    if (this -> input_device == KEYBOARD)
    {
        for (size_t i = 0; i < 8; i++)
            this -> saved_state |= (keyboard_state[this -> sdl_scancode_translations[this -> mapping[i]]] ? 1 : 0) << i;
    }
    else if ((this -> input_device == CONTROLLER) && SDL_NumJoysticks () > 0)
    {
        this -> saved_state |= SDL_GameControllerGetButton (this -> controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) << BUTTON::DPAD_LEFT;
        this -> saved_state |= SDL_GameControllerGetButton (this -> controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) << BUTTON::DPAD_RIGHT;
        this -> saved_state |= SDL_GameControllerGetButton (this -> controller, SDL_CONTROLLER_BUTTON_DPAD_UP) << BUTTON::DPAD_UP;
        this -> saved_state |= SDL_GameControllerGetButton (this -> controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) << BUTTON::DPAD_DOWN;
        this -> saved_state |= SDL_GameControllerGetButton (this -> controller, SDL_CONTROLLER_BUTTON_START) << BUTTON::START;
        this -> saved_state |= SDL_GameControllerGetButton (this -> controller, SDL_CONTROLLER_BUTTON_BACK) << BUTTON::SELECT;
        this -> saved_state |= SDL_GameControllerGetButton (this -> controller, SDL_CONTROLLER_BUTTON_A) << BUTTON::A;
        this -> saved_state |= SDL_GameControllerGetButton (this -> controller, SDL_CONTROLLER_BUTTON_B) << BUTTON::B;

        auto x_axis = SDL_GameControllerGetAxis (this -> controller, SDL_CONTROLLER_AXIS_LEFTX);
        auto y_axis = SDL_GameControllerGetAxis (this -> controller, SDL_CONTROLLER_AXIS_LEFTY);
        this -> saved_state |= ((x_axis < -(this -> controller_deadzone)) ? 1 : 0) << BUTTON::DPAD_LEFT;
        this -> saved_state |= ((x_axis > (this -> controller_deadzone)) ? 1 : 0) << BUTTON::DPAD_RIGHT;
        this -> saved_state |= ((y_axis < -(this -> controller_deadzone)) ? 1 : 0) << BUTTON::DPAD_UP;
        this -> saved_state |= ((y_axis > (this -> controller_deadzone)) ? 1 : 0) << BUTTON::DPAD_DOWN;
    }
}

uint8_t joypad::read (uint16_t /*address*/, bool /*to_parent_bus*/) // NOLINT
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

uint8_t joypad::get_button (joypad::BUTTON button) const
{
    return (((this -> saved_state) >> button) & 1);
}

void joypad::change_type (joypad::INPUT_DEVICE new_input_device)
{
    this -> input_device = new_input_device;

    if (new_input_device == CONTROLLER && SDL_NumJoysticks () > 0)
    {
        SDL_JoystickEventState(SDL_ENABLE);
        this -> controller = SDL_GameControllerOpen (this -> controller_number);
    }
}
