//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#include "include/devices/device.h"

bool device::responds_to (uint16_t address)
{
    return (IS_BETWEEN (this -> lower_bound, address, this -> upper_bound));
}