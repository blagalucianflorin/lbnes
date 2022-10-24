//
// Created by lblaga on 04.09.2022.
//

#include "configurator.hpp"

configurator *configurator::instance = nullptr;

configurator::configurator (const std::string &config_file_name)
{
    std::ifstream fin (config_file_name);

    if (fin.good ())
    {
        fin.close ();
        this -> config = YAML::LoadFile (config_file_name);
    }

    this -> set_defaults ();
}

configurator &configurator::get_instance ()
{
    if (configurator::instance == nullptr)
        configurator::instance = new configurator ();

    return (*(configurator::instance));
}

void configurator::set_defaults ()
{
    if (!(this -> config)["rom"])
        (this -> config)["rom"] = std::string();

    if (!(this -> config)["fullscreen"])
        (this -> config)["fullscreen"] = false;

    if (!(this -> config)["display-fps"])
        (this -> config)["display-fps"] = false;

    if (!(this -> config)["speed"])
        (this -> config)["speed"] = 100;

    if (!(this -> config)["resolution"])
    {
        (this->config)["resolution"]           = YAML::Node();
        (this->config)["resolution"]["width"]  = 768;
        (this->config)["resolution"]["height"] = 720;
    }

    if (!(this -> config)["new_controller_replaces_player_one"])
        (this -> config)["new_controller_replaces_player_one"] = true;
}
