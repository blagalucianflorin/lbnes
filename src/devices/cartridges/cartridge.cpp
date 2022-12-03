//
// Created by lblaga on 19.03.2022.
//

#include <iostream>
#include "devices/cartridges/cartridge.hpp"

cartridge::cartridge (const std::string &file_path): device (0x4020, 0xFFFF)
{
    std::string file_format  = file_path.substr (file_path.rfind ('.'));

    (this -> sram).resize (0x2000);
    for (size_t i = 0; i < 0x2000; i++) (this -> sram)[i] = 0x00;

    if (supported_formats.find (file_format) == supported_formats.end ())
        throw cartridge_exception (std::string ("Unsupported file format: ") + file_format);

    read_file_function read_file = supported_formats[file_format];
    (this ->* read_file) (file_path);
}

/*
 * Memory mapping;
 *  0x6000 - 0x8000 -> SRAM
 *  Others          -> UNASSIGNED (throws)
 */
void cartridge::write (uint16_t address, uint8_t data, bool /*to_parent_bus*/) // NOLINT
{
    if (IS_BETWEEN (0x6000, address, 0x7FFF))
        ((this -> sram))[address - 0x6000] = data;
}

/*
 * Memory mapping:
 *  0x0000 - 0x2000 -> CHR
 *  0x6000 - 0x8000 -> SRAM
 *  0x8000 - 0xFFFF -> PRG
 *  Others          -> UNASSIGNED (throws)
 */
uint8_t cartridge::read (uint16_t address, bool /*from_parent_bus*/) // NOLINT
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


std::string cartridge::save_state()
{
    YAML::Node base_device_node = YAML::Load (device::save_state());
    YAML::Node final_node;

    final_node["type"] = "cartridge";
    final_node["data"] = YAML::Node ();

    final_node["data"]["base_device"] = base_device_node;

    final_node["data"]["program_memory"]   = base64::encode (this -> program_memory.data (),
                                                               this -> program_memory.size ());
    final_node["data"]["character_memory"]   = base64::encode (this -> character_memory.data (),
                                                               this -> character_memory.size ());
    final_node["data"]["sram_memory"]   = base64::encode (this -> sram.data (),
                                                          this -> sram.size ());

    final_node["data"]["mapper"]         = static_cast <size_t> (this -> mapper);
    final_node["data"]["mirroring_type"] = static_cast <size_t> (this -> mirroring_type);

    return (YAML::Dump(final_node));
}


void cartridge::load_state (std::string saved_state)
{
    YAML::Node saved_node = YAML::Load (saved_state)["data"];
    device::load_state (YAML::Dump(saved_node["base_device"]));
    
    auto decoded_program_memory = base64::decode (saved_node["program_memory"].as<std::string> ());
    this -> program_memory.resize (decoded_program_memory.size ());
    for (size_t i = 0; i < decoded_program_memory.size (); i++)
        (this -> program_memory)[i] = decoded_program_memory[i];

    auto decoded_character_memory = base64::decode (saved_node["character_memory"].as<std::string> ());
    this -> character_memory.resize (decoded_character_memory.size ());
    for (size_t i = 0; i < decoded_character_memory.size (); i++)
        (this -> character_memory)[i] = decoded_character_memory[i];

    auto decoded_sram_memory = base64::decode (saved_node["sram_memory"].as<std::string> ());
    this -> sram.resize (decoded_sram_memory.size ());
    for (size_t i = 0; i < decoded_sram_memory.size (); i++)
        (this -> sram)[i] = decoded_sram_memory[i];

    auto aux_mapper = saved_node["mapper"].as <size_t> ();
    this -> mapper = static_cast <MAPPER> (aux_mapper);
    auto aux_mirroring_type = saved_node["mirroring_type"].as <size_t> ();
    this -> mirroring_type = static_cast <MIRRORING_TYPE> (aux_mirroring_type);
}
