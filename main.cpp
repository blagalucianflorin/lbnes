#include <memory>
//#include <iomanip>
//
//#include "devices/cpu/6502.hpp"
//#include "devices/memories/memory.hpp"
//#include "devices/ppu/ppu.hpp"

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

//    cpu
//    auto nes_cpu = std::make_shared<cpu> ();
//
//    auto saved_state = nes_cpu -> save_state ();
//    std::cout << "Before:\n" << saved_state << std::endl << std::endl;
//
//    nes_cpu -> load_state (saved_state);
//    saved_state = nes_cpu -> save_state ();
//    std::cout << "After:\n" << saved_state << std::endl << std::endl;

//    memory
//    auto nes_memory = std::make_shared <memory> (0x0000, 0x000F);
//
//    for (size_t i = 0; i < 8; i++)
//        nes_memory -> write (i, i, false);
//
//    auto saved_state = nes_memory -> save_state ();
//    std::cout << "Before:\n" << saved_state << std::endl << std::endl;
//
//    nes_memory -> load_state (saved_state);
//    saved_state = nes_memory -> save_state ();
//    std::cout << "After:\n" << saved_state << std::endl << std::endl;
//
//    for (size_t i = 0; i < 8; i++)
//        if (nes_memory -> read (i, false) != i)
//            std::cout << "iu\n";

//    bus
//    auto nes_bus = std::make_shared <bus> (0x0000, 0x000F);
//
//    auto saved_state = nes_bus -> save_state ();
//
//    state::change_type (saved_state, "cpu_bus");
//
//    std::cout << "Before:\n" << saved_state << std::endl << std::endl;
//
//    nes_bus -> load_state (saved_state);
//    saved_state = nes_bus -> save_state ();
//    std::cout << "After:\n" << saved_state << std::endl << std::endl;

//    nes
//    auto nes_ppu = std::make_shared <ppu> ();
//
//    std::ofstream before ("before.txt");
//    std::ofstream after ("after.txt");
//
//    auto saved_state = nes_ppu -> save_state ();
//
//    before << saved_state;
//
//    nes_ppu -> load_state (saved_state);
//    saved_state = nes_ppu -> save_state ();
//    after << saved_state;
}
