//
// Created by lblaga on 26.03.2022.
//

#include <iostream>
#include "include/devices/ppu/oam.h"

oam::oam () : device (0x4014, 0x4014)
{
    this -> memory = new uint8_t[256];
}

void oam::write (uint16_t address, uint8_t data, bool to_parent_bus) // NOLINT
{
    this -> dma_register = data;

    this -> cpu -> wait (this -> cpu -> get_cycles_elapsed () % 2 == 0 ? 513 : 514);

    for (uint16_t i = this -> ppu -> read (0x2003); i < 256; i++)
    {
        if (this -> memory == nullptr)
            throw ppu_exception ("OAM: Memory not initialized");

        (this -> memory)[i] = this -> cpu -> read ((((data << 8) & 0xFF00) + i));
        this -> cpu -> write (0x2004, (this -> memory)[i]);
    }
}

uint8_t oam::read (uint16_t address, bool from_parent_bus) // NOLINT
{
    throw ppu_exception ("OAM: Tried to read from OAM DMA register");
}
