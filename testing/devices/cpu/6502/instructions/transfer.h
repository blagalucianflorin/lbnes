//
// Created by lblaga on 11.03.2021.
//

#ifndef NEMULATOR_TRANSFER_H
#define NEMULATOR_TRANSFER_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "include/bus/bus.h"
#include "include/devices/memories/memory.h"
#include "include/devices/cpu/6502.h"

struct instructions_transfer: testing::Test
{
    bus     *my_bus;
    cpu     *my_cpu;
    memory  *my_ram;

    instructions_transfer ();

    ~instructions_transfer () override;
};

#endif //NEMULATOR_TRANSFER_H
