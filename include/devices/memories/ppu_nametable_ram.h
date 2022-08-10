//
// Created by lblaga on 20.03.2022.
//

#ifndef NEMULATOR_PPU_NAMETABLE_RAM_H
#define NEMULATOR_PPU_NAMETABLE_RAM_H

#include "include/devices/memories/memory.h"

class ppu_nametable_ram : public memory
{
public:
    inline  ppu_nametable_ram () : memory (0x2000, 0x3EFF) {}

    ~ppu_nametable_ram () override = default;

    void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t read (uint16_t address, bool to_parent_bus = true) override; // NOLINT
};

#endif //NEMULATOR_PPU_NAMETABLE_RAM_H
