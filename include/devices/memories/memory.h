//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#ifndef NEMULATOR_MEMORY_H
#define NEMULATOR_MEMORY_H

#include "forwards/classes.h"
#include "devices/device.h"
#include "misc/base64.h"

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

    std::string save_state (const std::string& name) override;

    void        load_state (std::string saved_state) override;
};

#endif //NEMULATOR_MEMORY_H
