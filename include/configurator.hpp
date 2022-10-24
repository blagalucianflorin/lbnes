//
// Created by lblaga on 04.09.2022.
//

#ifndef LBNES_CONFIGURATOR_HPP
#define LBNES_CONFIGURATOR_HPP

#include <string>
#include <fstream>

#include "yaml-cpp/yaml.h"

class configurator
{
protected:
    static     configurator *instance;

    YAML::Node config;

    explicit   configurator (const std::string &config_file_name = "config.yaml");

    void       set_defaults ();

public:
    static configurator &get_instance ();

    inline YAML::Node   operator[] (const std::string &value) { return (config[value]); }
};

#endif //LBNES_CONFIGURATOR_HPP
