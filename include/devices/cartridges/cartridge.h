//
// Created by lblaga on 19.03.2022.
//

#ifndef NEMULATOR_CARTRIDGE_H
#define NEMULATOR_CARTRIDGE_H

#include "include/devices/device.h"
#include "include/devices/cartridges/exceptions/cartridge_exception.h"

#include "include/misc/macros.h"

#include <string>
#include <fstream>
#include <cassert>
#include <map>
#include <cstring>
#include <algorithm>
#include <memory>

class cartridge : public device
{
public:
    friend class ppu;

    // Supported mapper types
    enum MAPPER
    {
        NROM
    };

    enum MIRRORING_TYPE
    {
        VERTICAL,
        HORIZONTAL,
        FOUR_SCREEN
    };

#ifdef G_TESTING
public:
#else
    private:
#endif
    std::vector<uint8_t> program_memory;
    std::vector<uint8_t> character_memory;
    std::vector<uint8_t> sram;

    MAPPER         mapper         = NROM;
    MIRRORING_TYPE mirroring_type = HORIZONTAL;

    void read_ines (const std::string& file_path);

    // Supported file formats
    typedef void (cartridge::*read_file_function) (const std::string &);
    std::map <std::string, read_file_function> supported_formats =
    {
        {".ines", &cartridge::read_ines}
    };

public:
    explicit cartridge (const std::string &file_path);

    ~cartridge () override = default;

    void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t read (uint16_t address, bool from_parent_bus = true) override; // NOLINT

    inline void reset () {};

    inline MIRRORING_TYPE get_mirroring () { return (this -> mirroring_type); }
};

#endif //NEMULATOR_CARTRIDGE_H
