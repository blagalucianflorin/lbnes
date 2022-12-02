//
// Created by lblaga on 19.03.2022.
//

#include "devices/memories/ram.hpp"

void ram::write (uint16_t address, uint8_t data, bool to_parent_bus) // NOLINT
{
    memory::write(address & 0x07FF, data, to_parent_bus);
}

uint8_t ram::read (uint16_t address, bool to_parent_bus) // NOLINT
{
    return memory::read(address & 0x07FF, to_parent_bus);
}

std::string ram::save_state()
{
    YAML::Node final_node = YAML::Load (memory::save_state());

    final_node["type"] = "ram";

    return (YAML::Dump (final_node));
}


void ram::load_state (std::string saved_state)
{
    memory::load_state (saved_state);
}