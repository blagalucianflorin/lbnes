//
// Created by lblaga on 04.09.2022.
//

#include "options/configurator.hpp"


configurator *configurator::instance   = nullptr;
std::string  configurator::config_file = "config.yaml";


configurator::configurator ()
{
    std::ifstream fin (configurator::config_file);

    if (fin.good ())
    {
        fin.close ();
        this -> config = YAML::LoadFile (configurator::config_file);
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
        (this -> config)["rom"] = std::string ();

    if (!(this -> config)["fullscreen"])
        (this -> config)["fullscreen"] = false;

    if (!(this -> config)["display_fps"])
        (this -> config)["display_fps"] = false;

    if (!(this -> config)["speed"])
        (this -> config)["speed"] = 100;

    if (!(this -> config)["resolution"])
    {
        (this -> config)["resolution"]           = YAML::Node ();
        (this -> config)["resolution"]["width"]  = 768;
        (this -> config)["resolution"]["height"] = 720;
    }

    if (!(this -> config)["new_controller_replaces_player_one"])
        (this -> config)["new_controller_replaces_player_one"] = true;

    if (!(this -> config)["show_menu"])
        (this -> config)["show_menu"] = true;

    if (!(this -> config)["vsync"])
        (this -> config)["vsync"] = false;

    // Controls defaults
    if (!(this -> config)["joypads"])
    {
        (this -> config)["joypads"] = YAML::Node ();
        auto keyboard = YAML::Node ();
        keyboard["type"]   = "keyboard";
        keyboard["player"] = 1;

        keyboard["mapping"]               = YAML::Node ();
        keyboard["mapping"]["dpad-up"]    = "w";
        keyboard["mapping"]["dpad-down"]  = "s";
        keyboard["mapping"]["dpad-left"]  = "a";
        keyboard["mapping"]["dpad-right"] = "d";
        keyboard["mapping"]["select"]     = "j";
        keyboard["mapping"]["start"]      = "k";
        keyboard["mapping"]["a"]          = "n";
        keyboard["mapping"]["b"]          = "m";

        (this -> config)["joypads"].push_back (keyboard);
    }
}
