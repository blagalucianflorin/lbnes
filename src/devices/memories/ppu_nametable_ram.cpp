//
// Created by lblaga on 20.03.2022.
//

#include "devices/memories/ppu_nametable_ram.h"

void ppu_nametable_ram::write (uint16_t address, uint8_t data, bool to_parent_bus) // NOLINT
{
    memory::write(address & 0x0FFF, data, to_parent_bus);
}

uint8_t ppu_nametable_ram::read (uint16_t address, bool to_parent_bus) // NOLINT
{
    return memory::read(address & 0x0FFF, to_parent_bus);
}
