//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <cstdint>
#include <memory>
#include <utility>

#include "forwards/classes.hpp"
#include "misc/macros.h"
#include "misc/state.h"


class device: public state
{
public:
    device (uint16_t lower_bound, uint16_t upper_bound) : lower_bound (lower_bound), upper_bound (upper_bound) {}

    virtual ~device () = default;

    [[nodiscard]] inline uint16_t get_lower_bound () const { return (this -> lower_bound); }

    [[nodiscard]] inline uint16_t get_upper_bound () const { return (this -> upper_bound); }

    [[nodiscard]] inline bool is_memory () const { return (this -> is_mem); }

    inline void     set_lower_bound (uint16_t new_lower_bound) { this -> lower_bound = new_lower_bound; }

    inline void     set_upper_bound (uint16_t new_upper_bound) { this -> upper_bound = new_upper_bound; }

    inline void     set_parent_bus (const std::shared_ptr<bus>& new_bus) { this -> parent_bus = new_bus; };

    inline void     set_child_bus (const std::shared_ptr<bus>& new_bus) { this -> child_bus = new_bus; };

    virtual bool    responds_to (uint16_t address);

    virtual void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) = 0; // NOLINT

    virtual uint8_t read (uint16_t address, bool from_parent_bus = true) = 0; // NOLINT

    std::string     save_state() override;

    void            load_state (std::string saved_state) override;

protected:
    bool                  is_mem      = true;
    uint16_t              lower_bound;
    uint16_t              upper_bound;
    std::shared_ptr <bus> parent_bus;
    std::shared_ptr <bus> child_bus;
};

#endif //DEVICE_HPP
