//
// Created by lblaga on 20.03.2022.
//

#ifndef NEMULATOR_TEST_CARTRIDGE_H
#define NEMULATOR_TEST_CARTRIDGE_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "include/devices/cartridges/cartridge.h"

#define ROM_FILES_PATH "../testing/devices/cartridges/roms/ines/"

struct test_cartridge : testing::Test
{
    cartridge *my_cartridge;

    test_cartridge ();

    ~test_cartridge () override;
};

#endif //NEMULATOR_TEST_CARTRIDGE_H
