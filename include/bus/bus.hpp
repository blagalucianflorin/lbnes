//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#ifndef BUS_HPP
#define BUS_HPP

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <memory>

#include "forwards/classes.hpp"
#include "devices/device.hpp"
#include "misc/state.h"


class bus: public state, public std::enable_shared_from_this <bus>
{
private:
    uint16_t lower_bound = 0x0000;
    uint16_t upper_bound = 0xFFFF;

    std::vector <std::shared_ptr <device>> devices;

public:
    bus () = default;

    virtual ~bus () = default;

    bus (uint16_t lower_bound, uint16_t upper_bound) : lower_bound (lower_bound), upper_bound (upper_bound) {}

    void        add_device (const std::shared_ptr<device>& new_device);

    void        add_devices (const std::vector <std::shared_ptr<device>>& new_devices);

    void        write (const uint16_t &address, const uint8_t &data);

    uint8_t     read (const uint16_t &address);

    std::string save_state (const std::string& name) override;

    void        load_state (std::string saved_state) override;
};

#endif //BUS_HPP
