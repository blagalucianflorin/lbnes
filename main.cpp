#include <memory>

#include "nes.hpp"
#include "options/arguments_manager.hpp"


int main (int argc, char *argv[])
{
    auto exit = arguments_manager::process (argc, argv);

    if (exit)
        return (0);

    // Create a preexisting shared pointer to the nes instance for shared_from_this
    std::shared_ptr <class nes> nes (new class nes ());
    nes -> start ();

    return (0);
}
