//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#include "devices/device.hpp"

#include <iostream>


bool device::responds_to (uint16_t address)
{
    return (IS_BETWEEN (this -> lower_bound, address, this -> upper_bound));
}


std::string device::save_state()
{
    YAML::Node saved_node = YAML::Node ();

    saved_node["lower_bound"] = static_cast <size_t> (this -> lower_bound);
    saved_node["upper_bound"] = static_cast <size_t> (this -> upper_bound);
    saved_node["is_mem"]      = this -> is_mem;

    return (YAML::Dump (saved_node));
}


void device::load_state (std::string saved_state)
{
    YAML::Node saved_node = YAML::Load (saved_state);

    this -> lower_bound = saved_node["lower_bound"].as <uint16_t> ();
    this -> upper_bound = saved_node["upper_bound"].as <uint16_t> ();
    this -> is_mem      = saved_node["is_mem"].as <bool> ();
}
