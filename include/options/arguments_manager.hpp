//
// Created by blaga on 21-Oct-22.
//

#ifndef LBNES_ARGUMENTS_MANAGER_HPP
#define LBNES_ARGUMENTS_MANAGER_HPP

#include "configurator.hpp"
#include "misc/logger.hpp"
#include <iostream>


namespace arguments_manager
{
    void print_help ();

    int process (int argc, char **argv);
}

#endif //LBNES_ARGUMENTS_MANAGER_HPP
