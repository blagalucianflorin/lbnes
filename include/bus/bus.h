//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#ifndef NEMULATOR_BUS_H
#define NEMULATOR_BUS_H

#include <cstdint>
#include <vector>
#include <stdexcept>

#include "include/forwards/classes.h"
#include "include/devices/device.h"

class bus
{
private:
    uint16_t              lower_bound = 0x0000;
    uint16_t              upper_bound = 0xFFFF;
    std::vector <device*> devices;

public:
    bus () = default;

    ~bus ();

    bus (uint16_t lower_bound, uint16_t upper_bound) : lower_bound (lower_bound), upper_bound (upper_bound) {}

    void    add_device (device *new_device);

    void    add_devices (const std::vector <device*>& new_devices);

    void    write (uint16_t address, uint8_t data);

    uint8_t read (uint16_t address);
};

#endif //NEMULATOR_BUS_H