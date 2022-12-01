//
// Created by lblaga on 20.03.2022.
//

#include "devices/memories/ppu_palette_ram.hpp"

void ppu_palette_ram::write (uint16_t address, uint8_t data, bool to_parent_bus) // NOLINT
{
    address &= 0x001F;

    if (address == 0x0010) address = 0x0000;
    if (address == 0x0014) address = 0x0004;
    if (address == 0x0018) address = 0x0008;
    if (address == 0x001C) address = 0x000C;

    memory::write(address, data, to_parent_bus);
}

uint8_t ppu_palette_ram::read (uint16_t address, bool to_parent_bus) // NOLINT
{
    address &= 0x001F;

    if (address == 0x0010) address = 0x0000;
    if (address == 0x0014) address = 0x0004;
    if (address == 0x0018) address = 0x0008;
    if (address == 0x001C) address = 0x000C;

    return memory::read(address, to_parent_bus);
}
