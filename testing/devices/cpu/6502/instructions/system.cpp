//
// Created by lblaga on 11.03.2021.
//

#include "testing/devices/cpu/6502/instructions/system.h"

instructions_system::instructions_system ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

instructions_system::~instructions_system ()
{
    delete this -> my_bus;
}


TEST_F (instructions_system, NOP_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0xEA);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0001);
}

TEST_F (instructions_system, BRK_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x00);
    my_cpu -> write (0xFFFE, 0xCC);
    my_cpu -> write (0xFFFF, 0xDD);

    my_cpu -> write (0xCCDD, 0xA9);
    my_cpu -> write (0xCCDE, 0x69);

    my_cpu -> step ();
    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0x69);
}

// This actually executes a BRK first, then returns using RTI
TEST_F (instructions_system, RTI_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x00);
    my_cpu -> write (0xFFFE, 0xCC);
    my_cpu -> write (0xFFFF, 0xDD);
    my_cpu -> write (0xCCDD, 0x40);

    my_cpu -> step ();
    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0001);
}