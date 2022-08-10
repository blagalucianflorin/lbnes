//
// Created by lblaga on 11.03.2021.
//

#ifndef NEMULATOR_SYSTEM_H
#define NEMULATOR_SYSTEM_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "include/bus/bus.h"
#include "include/devices/memories/memory.h"
#include "include/devices/cpu/6502.h"

struct instructions_system: testing::Test
{
    bus     *my_bus;
    cpu     *my_cpu;
    memory  *my_ram;

    instructions_system ();

    ~instructions_system () override;
};

#endif //NEMULATOR_SYSTEM_H