//
// Created by lblaga on 11.03.2021.
//

#include "testing/devices/cpu/6502/instructions/transfer.h"

instructions_transfer::instructions_transfer ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

instructions_transfer::~instructions_transfer ()
{
    delete this -> my_bus;
}


TEST_F (instructions_transfer, TAX_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0xAA);
    my_cpu -> accumulator   = 0xDD;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0xDD);
}

TEST_F (instructions_transfer, TAY_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0xA8);
    my_cpu -> accumulator   = 0xDD;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> y_register, 0xDD);
}

TEST_F (instructions_transfer, TSX_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0xBA);
    my_cpu -> stack_pointer = 0xDD;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0xDD);
}

TEST_F (instructions_transfer, TXA_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x8A);
    my_cpu -> x_register    = 0xDD;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0xDD);
}

TEST_F (instructions_transfer, TXS_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x9A);
    my_cpu -> x_register    = 0xDD;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> stack_pointer, 0xDD);
}

TEST_F (instructions_transfer, TYA_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x98);
    my_cpu -> y_register    = 0xDD;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0xDD);
}