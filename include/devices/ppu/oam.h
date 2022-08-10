//
// Created by lblaga on 26.03.2022.
//

#ifndef NEMULATOR_OAM_H
#define NEMULATOR_OAM_H

#include "include/devices/device.h"

#include "include/forwards/classes.h"
#include "include/devices/cpu/6502.h"
#include "include/devices/ppu/ppu.h"
#include "include/devices/ppu/exceptions/ppu_exception.h"

class oam : public device
{
private:
    uint8_t   *memory      = nullptr;
    class cpu *cpu         = nullptr;
    class ppu *ppu         = nullptr;
    uint8_t   dma_register = 0x00;

public:
    oam ();

    void        write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t     read (uint16_t address, bool from_parent_bus = true) override; // NOLINT

    inline uint8_t *&dma () { return (this -> memory); }

    inline void    attach (class cpu *new_cpu) { this -> cpu = new_cpu; }

    inline void    attach (class ppu *new_ppu) { this -> ppu = new_ppu; }
};

#endif //NEMULATOR_OAM_H
