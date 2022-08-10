//
// Created by Blaga Lucian-Florin on 10-Mar-21.
//

#ifndef NEMULATOR_ADDRESSING_MODES_H
#define NEMULATOR_ADDRESSING_MODES_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "include/bus/bus.h"
#include "include/devices/memories/memory.h"
#include "include/devices/cpu/6502.h"

struct addressing_modes: testing::Test
{
    bus     *my_bus;
    cpu     *my_cpu;
    memory  *my_ram;

    addressing_modes ();

    ~addressing_modes () override;
};

#endif //NEMULATOR_ADDRESSING_MODES_H
