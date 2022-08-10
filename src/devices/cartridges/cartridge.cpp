//
// Created by lblaga on 19.03.2022.
//

#include <iostream>
#include "include/devices/cartridges/cartridge.h"

cartridge::cartridge (const std::string &file_path): device (0x4020, 0xFFFF)
{
    std::string file_format  = file_path.substr (file_path.rfind ('.'));

    this -> sram = new uint8_t[0x2000];
    for (int i = 0; i < 0x2000; i++) (this -> sram)[i] = 0x00;

    if (supported_formats.find (file_format) == supported_formats.end ())
        throw cartridge_exception (std::string ("Unsupported file format: ") + file_format);

    read_file_function read_file = supported_formats[file_format];
    (this ->* read_file) (file_path);
}

cartridge::~cartridge () noexcept
{
    delete [](this -> sram);
    delete [](this -> program_memory);
    delete [](this -> character_memory);
}

/*
 * Memory mapping;
 *  0x6000 - 0x8000 -> SRAM
 *  Others          -> UNASSIGNED (throws)
 */
void cartridge::write (uint16_t address, uint8_t data, bool to_parent_bus) // NOLINT
{
    if (IS_BETWEEN (0x6000, address, 0x7FFF))
        (this -> sram)[address - 0x6000] = data;
}

/*
 * Memory mapping:
 *  0x0000 - 0x2000 -> CHR
 *  0x6000 - 0x8000 -> SRAM
 *  0x8000 - 0xFFFF -> PRG
 *  Others          -> UNASSIGNED (throws)
 */
uint8_t cartridge::read (uint16_t address, bool from_parent_bus) // NOLINT
{
    if (IS_BETWEEN (0x8000, address, 0xFFFF))
        return ((this -> program_memory)[address - 0x8000]);
    else if (IS_BETWEEN (0x6000, address, 0x7FFF))
        return ((this -> sram)[address - 0x6000]);
    else if (IS_BETWEEN (0x0000, address, 0x1FFF))
        return ((this -> character_memory)[address]);
    else
        return (0);
}
