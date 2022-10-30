//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#ifndef NEMULATOR_BUS_H
#define NEMULATOR_BUS_H

#include <cstdint>
#include <vector>
#include <stdexcept>

#include "forwards/classes.h"
#include "devices/device.h"
#include "misc/state.h"

class bus: public state
{
private:
    uint16_t              lower_bound = 0x0000;
    uint16_t              upper_bound = 0xFFFF;
    std::vector <device*> devices;

public:
    bus () = default;

    ~bus () = default;

    bus (uint16_t lower_bound, uint16_t upper_bound) : lower_bound (lower_bound), upper_bound (upper_bound) {}

    void    add_device (device *new_device);

    void    add_devices (const std::vector <device*>& new_devices);

    void    write (const uint16_t &address, const uint8_t &data);

    uint8_t read (const uint16_t &address);

    std::string save_state (const std::string& name) override;

    void        load_state (std::string saved_state) override;
};

#endif //NEMULATOR_BUS_H
