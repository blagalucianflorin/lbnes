//
// Created by lblaga on 11.03.2021.
//

#ifndef NEMULATOR_LOAD_STORE_H
#define NEMULATOR_LOAD_STORE_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "include/bus/bus.h"
#include "include/devices/memories/memory.h"
#include "include/devices/cpu/6502.h"

struct instructions_load_store: testing::Test
{
    bus     *my_bus;
    cpu     *my_cpu;
    memory  *my_ram;

    instructions_load_store ();

    ~instructions_load_store () override;
};

#endif //NEMULATOR_LOAD_STORE_H
