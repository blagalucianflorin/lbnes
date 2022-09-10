#include "nes.h"

int main (int argc, char **argv)
{
    class nes nes (argc > 1 ? argv[1] : "");
    nes.start ();

    return (0);
}
