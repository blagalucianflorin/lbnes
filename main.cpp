#include "include/nes.h"


int main (int argc, char **argv)
{
    std::string  rom_file = argc > 1 ? argv[1] : "smb.nes";

    nes my_nes (rom_file);
    my_nes.start ();

//    SMB example
//
//    my_nes.toggle_joypad (1);
//
//    for(int i = 0; i < 60; i++)
//        my_nes.render_frame ();
//
//    for(int i = 0; i < 60; i++)
//    {
//        my_nes.set_button (joypad::START);
//        my_nes.render_frame ();
//    }
//
//    for(int i = 0; i < 60; i++)
//        my_nes.render_frame ();
//
//
//    for(int i = 0; i < 300; i++)
//    {
//        my_nes.set_button (joypad::DPAD_RIGHT);
//        my_nes.render_frame ();
//    }

    return (0);
}
