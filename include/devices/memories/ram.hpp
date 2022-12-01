//
// Created by lblaga on 19.03.2022.
//

#ifndef RAM_HPP
#define RAM_HPP

#include "devices/memories/memory.hpp"


class ram : public memory
{
public:
    inline  ram () : memory (0x0000, 0x1FFF) {}

    ~ram () override = default;

    void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t read (uint16_t address, bool to_parent_bus = true) override; // NOLINT
};

#endif //RAM_HPP
