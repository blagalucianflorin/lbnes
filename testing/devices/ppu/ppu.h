//
// Created by lblaga on 26.03.2022.
//

#ifndef NEMULATOR_PPU_H
#define NEMULATOR_PPU_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <SDL2/SDL.h>

#include "include/forwards/classes.h"

#include "include/devices/ppu/oam.h"
#include "include/devices/ppu/ppu.h"
#include "include/devices/memories/ppu_palette_ram.h"
#include "include/devices/memories/ppu_nametable_ram.h"
#include "include/bus/bus.h"
#include "include/devices/cpu/6502.h"

struct test_ppu : testing::Test
{
    ppu *my_ppu     = nullptr;
    oam *my_oam     = nullptr;
    bus *my_cpu_bus = nullptr;
    bus *my_ppu_bus = nullptr;
    cpu *my_cpu     = nullptr;

    ppu_palette_ram *my_palette_ram     = nullptr;
    ppu_nametable_ram *my_nametable_ram = nullptr;

    test_ppu *my_cartridge;

    test_ppu ();

    ~test_ppu () override;
};

#endif //NEMULATOR_PPU_H
