//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#include "bus/bus.h"


void bus::add_device (device *new_device)
{
    (this -> devices).push_back (new_device);
    new_device -> set_parent_bus (*this);
}

void bus::add_devices (const std::vector<device*>& new_devices)
{
    for (auto &new_device : new_devices)
        this -> add_device (new_device);
}

void bus::write (const uint16_t &address, const uint8_t &data)
{
//    if (!IS_BETWEEN (this -> lower_bound, address, this -> upper_bound))
//        throw std::invalid_argument ("Bus: Tried to write outside of addressable range");

    static device *current_device;

    for (size_t i = 0; i < this -> devices.size (); i++)
    {
        current_device = devices[i];

        if (current_device -> responds_to (address))
        {
            current_device -> write (address, data);

            return;
        }
    }
}

uint8_t bus::read (const uint16_t &address)
{
//    if (!IS_BETWEEN (this -> lower_bound, address, this -> upper_bound))
//        throw std::invalid_argument ("Bus: Tried to read from outside of addressable range");

    static device *current_device;

    for (size_t i = 0; i < this -> devices.size (); i++)
    {
        current_device = devices[i];

        if (current_device -> responds_to (address))
            return (current_device -> read (address));
    }

    return (0);
}

std::string bus::save_state (const std::string &name)
{
    YAML::Node final_node;

    final_node[name] = YAML::Node ();
    final_node[name]["lower_bound"] = this -> lower_bound;
    final_node[name]["upper_bound"] = this -> upper_bound;
    final_node[name]["devices"]     = YAML::Node ();

    for (auto device : this -> devices)
        final_node[name]["devices"].push_back (device -> save_state (typeid (*device).name ()));

    return (YAML::Dump(final_node));
}

void bus::load_state (std::string saved_state)
{
    YAML::Node saved_node = YAML::Load (saved_state.c_str ()).begin() -> second;

    this -> lower_bound = saved_node["lower_bound"].as<uint16_t>();
    this -> upper_bound = saved_node["upper_bound"].as<uint16_t>();

//    this -> devices.clear ();
//    for (auto device : saved_node["devices"])
//        this -> devices. push_back ()
}
