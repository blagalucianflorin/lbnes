//
// Created by lblaga on 11.03.2021.
//

#include "testing/devices/cpu/6502/instructions/stack.h"

instructions_stack::instructions_stack ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

instructions_stack::~instructions_stack ()
{
    delete this -> my_bus;
}


TEST_F (instructions_stack, PHA_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x48);
    my_cpu -> accumulator   = 0xDD;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> read (0x01FD), 0xDD);
    EXPECT_EQ (my_cpu -> stack_pointer, 0xFC);
}

TEST_F (instructions_stack, PHP_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x08);
    my_cpu -> flags_register    = 0xDD;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> read (0x01FD), 0xFD);
    EXPECT_EQ (my_cpu -> stack_pointer, 0xFC);
}


TEST_F (instructions_stack, PLA_IMP_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x68);
    my_cpu -> write (0x01FF, 0xDD);
    my_cpu -> stack_pointer = 0xFE;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0xDD);
    EXPECT_EQ (my_cpu -> stack_pointer, 0xFF);
}

TEST_F (instructions_stack, PLA_IMP_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0x68);
    my_cpu -> write (0x01FF, 0x00);
    my_cpu -> stack_pointer = 0xFE;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0x00);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
    EXPECT_EQ (my_cpu -> stack_pointer, 0xFF);
}

TEST_F (instructions_stack, PLA_IMP_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0x68);
    my_cpu -> write (0x01FF, 0xF0);
    my_cpu -> stack_pointer = 0xFE;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0xF0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
    EXPECT_EQ (my_cpu -> stack_pointer, 0xFF);
}


TEST_F (instructions_stack, PLP_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x28);
    my_cpu -> write (0x01FF, 0xFF);
    my_cpu -> stack_pointer = 0xFE;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> flags_register, 0xEF);
    EXPECT_EQ (my_cpu -> stack_pointer, 0xFF);
}