//
// Created by lblaga on 19.03.2022.
//

#include "include/devices/memories/ram.h"

void ram::write (uint16_t address, uint8_t data, bool to_parent_bus) // NOLINT
{
    memory::write(address & 0x07FF, data, to_parent_bus);
}

uint8_t ram::read (uint16_t address, bool to_parent_bus) // NOLINT
{
    return memory::read(address & 0x07FF, to_parent_bus);
}
