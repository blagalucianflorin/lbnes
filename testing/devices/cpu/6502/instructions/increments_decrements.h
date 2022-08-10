//
// Created by lblaga on 11.03.2021.
//

#ifndef NEMULATOR_INCREMENTS_DECREMENTS_H
#define NEMULATOR_INCREMENTS_DECREMENTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "include/bus/bus.h"
#include "include/devices/memories/memory.h"
#include "include/devices/cpu/6502.h"

struct instructions_increments_decrements: testing::Test
{
    bus     *my_bus;
    cpu     *my_cpu;
    memory  *my_ram;

    instructions_increments_decrements ();

    ~instructions_increments_decrements () override;
};

#endif //NEMULATOR_INCREMENTS_DECREMENTS_H
