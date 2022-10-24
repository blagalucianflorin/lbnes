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
    virtual std::string save_state (const std::string& name) = 0;

    virtual void        load_state (std::string saved_state) = 0;
};

#endif //LBNES_STATE_H
