//
// Created by lblaga on 11.03.2021.
//

#ifndef NEMULATOR_STATUS_H
#define NEMULATOR_STATUS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "include/bus/bus.h"
#include "include/devices/memories/memory.h"
#include "include/devices/cpu/6502.h"

struct instructions_status: testing::Test
{
    bus     *my_bus;
    cpu     *my_cpu;
    memory  *my_ram;

    instructions_status ();

    ~instructions_status () override;
};

#endif //NEMULATOR_STATUS_H
