//
// Created by lblaga on 19.03.2022.
//

#ifndef NEMULATOR_RAM_H
#define NEMULATOR_RAM_H

#include "include/devices/memories/memory.h"

class ram : public memory
{
public:
    inline  ram () : memory (0x0000, 0x1FFF) {}

    ~ram () override = default;

    void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t read (uint16_t address, bool to_parent_bus = true) override; // NOLINT
};

#endif //NEMULATOR_RAM_H
