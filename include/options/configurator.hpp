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

    explicit   configurator ();

    void       set_defaults ();

public:
    static configurator &get_instance ();

    inline YAML::Node   operator[] (const std::string &value) { return (this -> config[value]); }

    inline YAML::Node   get_whole_config () { return (this -> config); }

    static std::string config_file;
};

#endif //LBNES_CONFIGURATOR_HPP
