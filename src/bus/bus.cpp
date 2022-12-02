//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#include "bus/bus.hpp"


void bus::add_device (const std::shared_ptr<device>& new_device)
{
    (this -> devices).push_back (new_device);
    new_device -> set_parent_bus (shared_from_this ());
}

void bus::add_devices (const std::vector <std::shared_ptr<device>>& new_devices)
{
    for (auto &new_device : new_devices)
        this -> add_device (new_device);
}

void bus::write (const uint16_t &address, const uint8_t &data)
{
//    if (!IS_BETWEEN (this -> lower_bound, address, this -> upper_bound))
//        throw std::invalid_argument ("Bus: Tried to write outside addressable range");

    for (const auto& device : this -> devices)
        if (device -> responds_to (address))
            device -> write (address, data);
}

uint8_t bus::read (const uint16_t &address)
{
//    if (!IS_BETWEEN (this -> lower_bound, address, this -> upper_bound))
//        throw std::invalid_argument ("Bus: Tried to read from outside addressable range");

    for (const auto& device : this -> devices)
        if (device -> responds_to (address))
            return (device -> read (address));

    return (0);
}

std::string bus::save_state()
{
    YAML::Node final_node;

    final_node["type"] = "bus";
    final_node["data"] = YAML::Node ();
    final_node["data"]["lower_bound"] = this -> lower_bound;
    final_node["data"]["upper_bound"] = this -> upper_bound;

    return (YAML::Dump(final_node));
}

void bus::load_state (std::string saved_state)
{
    YAML::Node saved_node = YAML::Load (saved_state)["data"];

    this -> lower_bound = saved_node["lower_bound"].as<uint16_t>();
    this -> upper_bound = saved_node["upper_bound"].as<uint16_t>();
}
