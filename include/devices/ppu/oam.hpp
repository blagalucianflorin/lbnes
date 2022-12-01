//
// Created by lblaga on 26.03.2022.
//

#ifndef OAM_HPP
#define OAM_HPP

#include "devices/device.hpp"

#include "forwards/classes.hpp"
#include "devices/cpu/6502.hpp"
#include "devices/ppu/ppu.hpp"
#include "devices/ppu/exceptions/ppu_exception.h"

// Unused
class oam : public device
{
public:
    oam ();

    void        write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t     read (uint16_t address, bool from_parent_bus = true) override; // NOLINT

    inline uint8_t *&dma () { return (this -> memory); }

    inline void    attach (class cpu *new_cpu) { this -> cpu = new_cpu; }

    inline void    attach (class ppu *new_ppu) { this -> ppu = new_ppu; }

private:
    uint8_t   *memory = nullptr;
    class cpu *cpu    = nullptr;
    class ppu *ppu    = nullptr;
};

#endif //OAM_HPP
