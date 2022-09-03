#include "nes.h"

int main (int argc, char **argv)
{
    std::string  rom_file = argc > 1 ? argv[1] : "smb.nes";

    nes my_nes (rom_file);
    my_nes.start ();

    return (0);
}
