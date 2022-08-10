//
// Created by lblaga on 11.03.2021.
//

#ifndef NEMULATOR_LOGICAL_H
#define NEMULATOR_LOGICAL_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "include/bus/bus.h"
#include "include/devices/memories/memory.h"
#include "include/devices/cpu/6502.h"

struct instructions_logical: testing::Test
{
    bus     *my_bus;
    cpu     *my_cpu;
    memory  *my_ram;

    instructions_logical ();

    ~instructions_logical () override;
};

#endif //NEMULATOR_LOGICAL_H
