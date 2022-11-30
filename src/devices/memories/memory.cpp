//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#include <iostream>

#include "devices/memories/memory.h"


memory::memory (uint16_t lower_bound, uint16_t upper_bound) : device (lower_bound, upper_bound)
{
    this -> internal_memory.resize (upper_bound - lower_bound);

    for (uint16_t i = 0; i < upper_bound - lower_bound; i++)
        (this -> internal_memory)[i] = 0x00;
}


std::string memory::save_state (const std::string& name)
{
    YAML::Node base_device_node = YAML::Load (device::save_state ("base_device"));
    YAML::Node final_node;

    final_node[name]                    = YAML::Node ();
    final_node[name]["memory_device"]   = YAML::Node ();
    final_node[name]["internal_memory"] = base64::encode (this -> internal_memory.data (),
                                                          this -> upper_bound - this -> lower_bound);

    for (auto pair : base_device_node["base_device"])
        final_node[name]["base_device"][pair.first] = pair.second;

    return (YAML::Dump (final_node));
}


void memory::load_state (std::string saved_state)
{
    YAML::Node saved_node     = YAML::Load (saved_state.c_str ()).begin() -> second;
    auto       decoded_memory = base64::decode (saved_node["internal_memory"].as<std::string> ());
    device::load_state (YAML::Dump(saved_node["base_device"]));

    this -> internal_memory.resize (upper_bound - lower_bound);
    for (uint16_t i = 0; i < this -> upper_bound - this -> lower_bound; i++)
        (this -> internal_memory)[i] = decoded_memory[i];
}
