//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#include "include/bus/bus.h"
#include "include/misc/macros.h"

void bus::add_device (device *new_device)
{
    (this -> devices).push_back (new_device);
    new_device -> set_parent_bus (*this);
}

void bus::add_devices (const std::vector<device*>& new_devices)
{
    for (auto &new_device : new_devices)
        this -> add_device (new_device);
}

void bus::write (uint16_t address, uint8_t data)
{
    if (!IS_BETWEEN (this -> lower_bound, address, this -> upper_bound))
        throw std::invalid_argument ("Bus: Tried to write outside of addressable range");

    for (auto device : this -> devices)
        if (device -> responds_to (address) && device -> is_memory ())
        {
            device -> write (address, data);
            break;
        }
}

uint8_t bus::read (uint16_t address)
{
    if (!IS_BETWEEN (this -> lower_bound, address, this -> upper_bound))
        throw std::invalid_argument ("Bus: Tried to read from outside of addressable range");

    for (auto device : this -> devices)
        if (device -> responds_to (address) && device -> is_memory ())
            return (device -> read (address));

    return (0);
}