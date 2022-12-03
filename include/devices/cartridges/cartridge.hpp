//
// Created by lblaga on 19.03.2022.
//

#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include "devices/device.hpp"
#include "devices/cartridges/exceptions/cartridge_exception.h"

#include "misc/macros.h"
#include "misc/logger.hpp"
#include "misc/base64.h"

#include <string>
#include <fstream>
#include <cassert>
#include <map>
#include <cstring>
#include <algorithm>
#include <memory>
#include <vector>


class cartridge : public device
{
public:
    friend class ppu;

    enum MIRRORING_TYPE
    {
        VERTICAL,
        HORIZONTAL,
        FOUR_SCREEN
    };

    explicit cartridge (const std::string &file_path);

    ~cartridge () override = default;

    void        write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t     read (uint16_t address, bool from_parent_bus = true) override; // NOLINT

    inline void reset () {};

    std::string save_state() override;

    void        load_state (std::string saved_state) override;

    [[nodiscard]] inline MIRRORING_TYPE get_mirroring () const { return (this -> mirroring_type); }

private:
    // Supported mapper types
    enum MAPPER
    {
        NROM
    };

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
        {".nes", &cartridge::read_ines},
        {".ines", &cartridge::read_ines}
    };
};

#endif //CARTRIDGE_HPP
