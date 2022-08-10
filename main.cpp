#include "include/nes.h"


int main (int argc, char **argv)
{
    std::string  rom_file = argc > 1 ? argv[1] : "";

    rom_file = "../roms/smb.ines";

    nes my_nes (rom_file);
//    my_nes.start ();

    my_nes.toggle_joypad (1);

    for(int i = 0; i < 60; i++)
        my_nes.render_frame ();

    for(int i = 0; i < 60; i++)
    {
        my_nes.set_button (joypad::START);
        my_nes.render_frame ();
    }

    for(int i = 0; i < 60; i++)
        my_nes.render_frame ();


    for(int i = 0; i < 300; i++)
    {
        my_nes.set_button (joypad::DPAD_RIGHT);
        my_nes.render_frame ();
    }

    my_nes.toggle_joypad (1);

//    my_nes.start ();

    return (0);
}
