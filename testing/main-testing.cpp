//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "testing/devices/cpu/6502/addressing_modes/addressing_modes.h"
#include "testing/devices/cpu/6502/instructions/instructions.h"
#include "testing/devices/cartridges/cartridge.h"

int main (int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return (RUN_ALL_TESTS());
}
