//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>

#include "forwards/classes.hpp"
#include "devices/device.hpp"
#include "misc/base64.h"


class memory : public device
{
public:
    memory (uint16_t lower_bound, uint16_t upper_bound);

    ~memory () noexcept override = default;

    inline void    write (uint16_t address, uint8_t data, bool /*to_parent_bus = true*/) override // NOLINT
    {
        internal_memory[address - (this -> lower_bound)] = data;
    }

    inline uint8_t read (uint16_t address, bool /*to_parent_bus = true*/) override // NOLINT
    {
        return (internal_memory[address - (this -> lower_bound)]);
    }

    std::string save_state() override;

    void        load_state (std::string saved_state) override;

private:
    std::vector <uint8_t> internal_memory;
};

#endif //MEMORY_HPP
