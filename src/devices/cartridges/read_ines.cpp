//
// Created by lblaga on 20.03.2022.
//

#include "devices/cartridges/cartridge.hpp"

void cartridge::read_ines (const std::string& file_path)
{
    char          format_check[5],  padding[8];
    char          program_size, character_size, program_ram_size;
    char          control_byte_one, control_byte_two;
    uint8_t       aux;
    std::ifstream file (file_path, std::ios_base::binary);

    if (!file.good ())
    {
        LOGGER_ERROR ("Provided ROM file doesn't exist.");
        throw cartridge_exception("NES: ROM file doesn't exist");
    }

    // Read header and check integrity
    format_check[4] = padding[7] = '\0';
    file.read (format_check, 4);
    file.get (program_size);
    file.get (character_size);
    file.get (control_byte_one);
    file.get (control_byte_two);
    file.get (program_ram_size);
    file.read (padding, 7);

    if (std::string (format_check) != "NES\x1A" || std::string (padding) != "\0\0\0\0\0\0\0\0") // NOLINT
        throw cartridge_exception ("iNES: Corrupted file header");

    (this -> program_memory).resize (std::max (static_cast <uint8_t> (program_size), static_cast <uint8_t> (2)) * 0x4000);
    (this -> character_memory).resize (static_cast <uint8_t> (character_size) * 0x2000);

    if ((control_byte_two & 0b00001111) != 0)
        throw cartridge_exception ("iNES: Only iNES 1.0 is supported");

    if ((control_byte_one & 0b00001000) != 0)
        this -> mirroring_type = FOUR_SCREEN;
    else if ((control_byte_one & 0b00000001) != 0)
        this -> mirroring_type = VERTICAL;
    else
        this -> mirroring_type = HORIZONTAL;

    // TODO find a more easily extendable mechanism for checking mappers
    if ((((control_byte_one & 0xF0) >> 4) | (control_byte_two & 0xF0) << 4) != 0)
        throw cartridge_exception ("iNES: Unsupported mapper");

    // Read contents
    for (size_t i = 0; i < static_cast <uint8_t> (program_size) * 0x4000; i++)
    {
        file.read (reinterpret_cast<char *> (&aux), 1);
        (this -> program_memory)[i] = aux;
    }
    // Check if trainer exists
    if ((control_byte_one & 0b00000010) != 0)
        for (size_t i = 0; i < 512; i++)
        {
            file.read (reinterpret_cast<char *> (&aux), 1);
            (this -> sram)[0x1000 + i] = aux;
        }
    for (size_t i = 0; i < static_cast <uint8_t> (character_size) * 0x2000; i++)
    {
        file.read (reinterpret_cast<char *> (&aux), 1);
        (this -> character_memory)[i] = aux;
    }

    // Mirror contents of first 16K of PRG is only one memory block is present
    if (program_size == 1)
        std::memcpy (this -> program_memory.data () + 0x4000, this -> program_memory.data (), 0x4000);

    this -> mapper = NROM;
}