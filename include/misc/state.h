//
// Created by lblaga on 11.09.2022.
//

#ifndef LBNES_STATE_H
#define LBNES_STATE_H

#include <string>

#include "yaml-cpp/yaml.h"

class state
{
public:
    virtual std::string save_state() = 0;

    virtual void        load_state (std::string saved_state) = 0;

    inline static void change_type (std::string &saved_state, const std::string &new_type)
    {
        YAML::Node final_node = YAML::Load (saved_state);

        final_node["type"] = new_type;

        saved_state = YAML::Dump (final_node);
    }
};

#endif //LBNES_STATE_H
