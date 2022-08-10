//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#ifndef NEMULATOR_MEMORY_H
#define NEMULATOR_MEMORY_H

#include "include/forwards/classes.h"
#include "include/devices/device.h"

class memory : public device
{
private:
    uint8_t *internal_memory;

public:
    memory (uint16_t lower_bound, uint16_t upper_bound);

    ~memory () noexcept override;

    inline void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) // NOLINT
                            override
    {
        internal_memory[address - (this -> lower_bound)] = data;
    }

    inline uint8_t read (uint16_t address, bool to_parent_bus = true) override // NOLINT
    {
        return (internal_memory[address - (this -> lower_bound)]);
    }
};

#endif //NEMULATOR_MEMORY_H