//
// Created by lblaga on 04.09.2022.
//

#include "configurator.h"

configurator *configurator::instance = nullptr;

configurator::configurator (const std::string &config_file_name)
{
    std::ifstream fin ("config.yaml");
    if (fin.good ())
    {
        fin.close ();
        this -> config = YAML::LoadFile (config_file_name);
    }
}

configurator &configurator::get_instance ()
{
    if (configurator::instance == nullptr)
        configurator::instance = new configurator ();

    return (*(configurator::instance));
}