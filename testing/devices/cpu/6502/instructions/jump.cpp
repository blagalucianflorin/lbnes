//
// Created by lblaga on 11.03.2021.
//

#include "testing/devices/cpu/6502/instructions/jump.h"

instructions_jump::instructions_jump ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

instructions_jump::~instructions_jump ()
{
    delete this -> my_bus;
}


TEST_F (instructions_jump, JMP_ABS) // NOLINT
{
    my_cpu -> write (0x0000, 0x4C);
    my_cpu -> write (0x0001, 0xAA);
    my_cpu -> write (0x0002, 0xBB);

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> program_counter, 0xBBAA);
}


TEST_F (instructions_jump, JMP_IND_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x6C);
    my_cpu -> write (0x0001, 0xBB);
    my_cpu -> write (0x0002, 0xAA);
    my_cpu -> write (0xAABB, 0xDD);
    my_cpu -> write (0xAABC, 0xCC);

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> program_counter, 0xCCDD);
}

TEST_F (instructions_jump, JMP_IND_HARDWARE_BUG) // NOLINT
{
    my_cpu -> write (0x0000, 0x6C);
    my_cpu -> write (0x0001, 0xFF);
    my_cpu -> write (0x0002, 0xAA);
    my_cpu -> write (0xAAFF, 0xDD);
    my_cpu -> write (0xAA00, 0xCC);

    my_cpu -> clock();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0xCCDD);
}


TEST_F (instructions_jump, JSR_ABS) // NOLINT
{
    my_cpu -> write (0x0000, 0x20);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x0002, 0xDD);

    my_cpu -> clock();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> read (0x01FD), 0x00);
    EXPECT_EQ (my_cpu -> read (0x01FC), 0x02);
    EXPECT_EQ (my_cpu -> stack_pointer, 0xFB);
    EXPECT_EQ (my_cpu -> program_counter, 0xDDCC);
}


TEST_F (instructions_jump, RTS_IMP_AFTER_JSR_ABS) // NOLINT
{
    my_cpu -> write (0x0000, 0x20);
    my_cpu -> write (0x0001, 0xDD);
    my_cpu -> write (0x0002, 0xCC);

    my_cpu -> clock();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    my_cpu -> write (0xCCDD, 0x60);

    my_cpu -> clock();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> stack_pointer, 0xFD);
    EXPECT_EQ (my_cpu -> program_counter, 0x0003);
}
