//
// Created by lblaga on 11.03.2021.
//

#ifndef NEMULATOR_SHIFTS_H
#define NEMULATOR_SHIFTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "include/bus/bus.h"
#include "include/devices/memories/memory.h"
#include "include/devices/cpu/6502.h"

struct instructions_shifts: testing::Test
{
    bus     *my_bus;
    cpu     *my_cpu;
    memory  *my_ram;

    instructions_shifts ();

    ~instructions_shifts () override;
};

#endif //NEMULATOR_SHIFTS_H
