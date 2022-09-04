//
// Created by lblaga on 04.09.2022.
//

#ifndef LBNES_CONFIGURATOR_H
#define LBNES_CONFIGURATOR_H

#include <string>
#include <fstream>

#if WIN32
#define YAML_CPP_STATIC_DEFINE
#endif
#include "yaml-cpp/yaml.h"

class configurator
{
protected:
    static configurator *instance;

    explicit configurator (const std::string &config_file_name = "config.yaml");

    YAML::Node config;

public:
    static configurator &get_instance ();

    inline YAML::Node operator[] (const std::string &value) { return (config[value]); }
};

#endif //LBNES_CONFIGURATOR_H
