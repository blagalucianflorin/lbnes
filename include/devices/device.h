//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#ifndef NEMULATOR_DEVICE_H
#define NEMULATOR_DEVICE_H

#include <cstdint>

#include "forwards/classes.h"
#include "misc/macros.h"

class device
{
protected:
    uint16_t    lower_bound;
    uint16_t    upper_bound;
    bus         *parent_bus = nullptr;
    bus         *child_bus  = nullptr;
    bool        is_mem      = true;

public:
    device (uint16_t lower_bound, uint16_t upper_bound) : lower_bound (lower_bound), upper_bound (upper_bound) {}

    virtual ~device () = default;

    inline uint16_t get_lower_bound () { return (this -> lower_bound); }

    inline uint16_t get_upper_bound () { return (this -> upper_bound); }

    inline void     set_lower_bound (uint16_t new_lower_bound) { this -> lower_bound = new_lower_bound; }

    inline void     set_upper_bound (uint16_t new_upper_bound) { this -> upper_bound = new_upper_bound; }

    inline void     set_parent_bus (bus &new_bus) { this -> parent_bus = &new_bus; };

    inline void     set_child_bus (bus &new_bus) { this -> child_bus = &new_bus; };

    inline bool     is_memory () { return (this -> is_mem); }

    virtual bool    responds_to (uint16_t address);

    virtual void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) = 0; // NOLINT

    virtual uint8_t read (uint16_t address, bool from_parent_bus = true) = 0; // NOLINT
};

#endif //NEMULATOR_DEVICE_H
