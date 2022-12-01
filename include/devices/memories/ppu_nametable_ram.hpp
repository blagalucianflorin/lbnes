//
// Created by lblaga on 20.03.2022.
//

#ifndef PPU_NAMETABLE_RAM_HPP
#define PPU_NAMETABLE_RAM_HPP

#include "devices/memories/memory.hpp"


class ppu_nametable_ram : public memory
{
public:
    inline  ppu_nametable_ram () : memory (0x2000, 0x3EFF) {}

    ~ppu_nametable_ram () override = default;

    void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t read (uint16_t address, bool to_parent_bus = true) override; // NOLINT
};

#endif //PPU_NAMETABLE_RAM_HPP
