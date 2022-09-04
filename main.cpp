#include "nes.h"

int main (int argc, char **argv)
{
    std::string rom_file = argc > 1 ? argv[1] : "smb.nes";

    class nes nes (rom_file);
    nes.start ();

    return (0);
}
