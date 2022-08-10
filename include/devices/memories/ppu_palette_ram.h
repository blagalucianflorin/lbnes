//
// Created by lblaga on 20.03.2022.
//

#ifndef NEMULATOR_PPU_PALETTE_RAM_H
#define NEMULATOR_PPU_PALETTE_RAM_H

#include "include/devices/memories/memory.h"

class ppu_palette_ram : public memory
{
public:
    inline  ppu_palette_ram () : memory (0x3F00, 0x3FFF) {}

    ~ppu_palette_ram () override = default;

    void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t read (uint16_t address, bool to_parent_bus = true) override; // NOLINT
};

#endif //NEMULATOR_PPU_PALETTE_RAM_H
