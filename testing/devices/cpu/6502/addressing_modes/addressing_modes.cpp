//
// Created by Blaga Lucian-Florin on 10-Mar-21.
//

#include "testing/devices/cpu/6502/addressing_modes/addressing_modes.h"

addressing_modes::addressing_modes ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

addressing_modes::~addressing_modes ()
{
    delete this -> my_bus;
}


TEST_F (addressing_modes, IMP_SEC) // NOLINT
{
    my_cpu -> write (0x0000, 0x38);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
}

TEST_F (addressing_modes, ACC_ASL) // NOLINT
{
    my_cpu -> write (0x0000, 0x0A);
    my_cpu -> accumulator = 2;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 4);
}

TEST_F (addressing_modes, IMM_LDA) // NOLINT
{
    my_cpu -> write (0x0000, 0xA9);
    my_cpu -> write (0x0001, 0xFF);
    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();
    EXPECT_EQ (my_cpu -> accumulator, 0xFF);
    EXPECT_EQ (my_cpu -> cycles_left, 0);
}

TEST_F (addressing_modes, ZPG_LDA) // NOLINT
{
    my_cpu -> write (0x0000, 0xA5);
    my_cpu -> write (0x0001, 0x69);
    my_cpu -> write (0x0069, 0x41);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0x41);
    EXPECT_EQ (my_cpu -> cycles_left, 0);
}

TEST_F (addressing_modes, ZPX_LDA) // NOLINT
{
    my_cpu -> write (0x0000, 0xB5);
    my_cpu -> write (0x0001, 0x09);
    my_cpu -> x_register = 0x60;
    my_cpu -> write (0x0069, 0x41);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0x41);
    EXPECT_EQ (my_cpu -> cycles_left, 0);
}

TEST_F (addressing_modes, ZPY_LDX) // NOLINT
{
    my_cpu -> write (0x0000, 0xB6);
    my_cpu -> write (0x0001, 0x09);
    my_cpu -> y_register = 0x60;
    my_cpu -> write (0x0069, 0x41);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0x41);
    EXPECT_EQ (my_cpu -> cycles_left, 0);
}

TEST_F (addressing_modes, REL_BCC) // NOLINT
{
    my_cpu -> write (0x0000, 0x90);
    my_cpu -> write (0x0001, 0x03);
    my_cpu -> write (0x0003, 0x41);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0005);
    EXPECT_EQ (my_cpu -> cycles_left, 0);
}

TEST_F (addressing_modes, ABS_LDA) // NOLINT
{
    my_cpu -> write (0x0000, 0xAD);
    my_cpu -> write (0x0001, 0xAA);
    my_cpu -> write (0x0002, 0xAA);
    my_cpu -> write (0xAAAA, 0x41);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0x41);
    EXPECT_EQ (my_cpu -> cycles_left, 0);
}

TEST_F (addressing_modes, ABX_LDA) // NOLINT
{
    my_cpu -> write (0x0000, 0xBD);
    my_cpu -> write (0x0001, 0xAA);
    my_cpu -> write (0x0002, 0xA9);
    my_cpu -> write (0xAAAA, 0x41);
    my_cpu -> x_register = 0x01;
    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();
    EXPECT_EQ (my_cpu -> accumulator, 0x00);
    EXPECT_EQ (my_cpu -> cycles_left, 0);
}

TEST_F (addressing_modes, ABY_LDA) // NOLINT
{
    my_cpu -> write (0x0000, 0xB9);
    my_cpu -> write (0x0001, 0xAA);
    my_cpu -> write (0x0002, 0xA9);
    my_cpu -> write (0xAAAA, 0x41);
    my_cpu -> y_register = 0x01;
    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();
    EXPECT_EQ (my_cpu -> accumulator, 0x00);
    EXPECT_EQ (my_cpu -> cycles_left, 0);
}

TEST_F (addressing_modes, IND_JMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x6C);
    my_cpu -> write (0x0001, 0xCD);
    my_cpu -> write (0x0002, 0xAB);

    my_cpu -> write (0xABCD, 0xDD);
    my_cpu -> write (0xABCE, 0xFF);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0xFFDD);
    EXPECT_EQ (my_cpu -> cycles_left, 0);
}

TEST_F (addressing_modes, IDX_LDA) // NOLINT
{
    my_cpu -> write (0x0000, 0xA1);
    my_cpu -> write (0x0001, 0x40);
    my_cpu -> x_register = 0x06;

    my_cpu -> write (0x0046, 0xAA);
    my_cpu -> write (0x0047, 0xAA);
    my_cpu -> write (0xAAAA, 0x41);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0x41);
    EXPECT_EQ (my_cpu -> cycles_left, 0);
}

TEST_F (addressing_modes, IDY_LDA) // NOLINT
{
    my_cpu -> write (0x0000, 0xB1);
    my_cpu -> write (0x0001, 0x40);
    my_cpu -> y_register = 0x0A;

    my_cpu -> write (0x0040, 0xA0);
    my_cpu -> write (0x0041, 0xAA);
    my_cpu -> write (0xAAAA, 0x41);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0x41);
    EXPECT_EQ (my_cpu -> cycles_left, 0);
}
