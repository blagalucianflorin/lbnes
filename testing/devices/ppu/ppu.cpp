//
// Created by lblaga on 26.03.2022.
//

#include "testing/devices/ppu/ppu.h"

test_ppu::test_ppu ()
{
    this -> my_ppu = new ppu ();
    this -> my_cpu = new cpu ();

//    ppu *my_ppu     = nullptr;
//    oam *my_oam     = nullptr;
//    bus *my_cpu_bus = nullptr;
//    bus *my_ppu_bus = nullptr;
//    cpu *my_cpu     = nullptr;
//
//    ppu_palette_ram *my_palette_ram     = nullptr;
//    ppu_nametable_ram *my_nametable_ram = nullptr;
}

test_ppu::~test_ppu ()
{

}
