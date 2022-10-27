#include "nes.h"
#include "options/arguments_manager.hpp"

int main (int argc, char *argv[])
{
    auto exit = arguments_manager::process (argc, argv);

    if (exit)
        return (0);

    class nes nes;
    nes.start ();

    return (0);
}
