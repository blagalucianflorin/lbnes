//
// Created by lblaga on 11.03.2021.
//

#ifndef NEMULATOR_BRANCH_H
#define NEMULATOR_BRANCH_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "include/bus/bus.h"
#include "include/devices/memories/memory.h"
#include "include/devices/cpu/6502.h"

struct instructions_branch: testing::Test
{
    bus     *my_bus;
    cpu     *my_cpu;
    memory  *my_ram;

    instructions_branch ();

    ~instructions_branch () override;
};

#endif //NEMULATOR_BRANCH_H
