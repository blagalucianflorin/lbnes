//
// Created by lblaga on 11.03.2021.
//

#include "testing/devices/cpu/6502/instructions/logical.h"

instructions_logical::instructions_logical ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

instructions_logical::~instructions_logical ()
{
    delete this -> my_bus;
}


TEST_F (instructions_logical, AND_IMM_ALL_BITS_MATCH) // NOLINT
{
    my_cpu -> write (0x0000, 0x29);
    my_cpu -> write (0x0001, 0b10101010);
    my_cpu -> accumulator = 0b10101010;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b10101010);
}

TEST_F (instructions_logical, AND_IMM_NO_BITS_MATCH) // NOLINT
{
    my_cpu -> write (0x0000, 0x29);
    my_cpu -> write (0x0001, 0b01010101);
    my_cpu -> accumulator = 0b10101010;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b00000000);
}

TEST_F (instructions_logical, AND_IMM_SOME_BITS_MATCH) // NOLINT
{
    my_cpu -> write (0x0000, 0x29);
    my_cpu -> write (0x0001, 0b10100000);
    my_cpu -> accumulator = 0b10101010;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b10100000);
}


TEST_F (instructions_logical, EOR_IMM_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x49);
    my_cpu -> write (0x0001, 0b00001111);
    my_cpu -> accumulator = 0b11111111;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b11110000);
}

TEST_F (instructions_logical, EOR_IMM_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0x49);
    my_cpu -> write (0x0001, 0b11111111);
    my_cpu -> accumulator = 0b11111111;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b00000000);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_logical, EOR_IMM_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0x49);
    my_cpu -> write (0x0001, 0b10000000);
    my_cpu -> accumulator = 0b00000000;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b10000000);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_logical, ORA_IMM_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x09);
    my_cpu -> write (0x0001, 0b10101010);
    my_cpu -> accumulator = 0b01010101;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b11111111);
}

TEST_F (instructions_logical, ORA_IMM_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0x09);
    my_cpu -> write (0x0001, 0b00000000);
    my_cpu -> accumulator = 0b00000000;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b00000000);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_logical, ORA_IMM_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0x09);
    my_cpu -> write (0x0001, 0b10000000);
    my_cpu -> accumulator = 0b00000000;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b10000000);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_logical, BIT_ZPG_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0x24);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 0b00000000);
    my_cpu -> accumulator = 0b11111111;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b11111111);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_logical, BIT_ZPG_OVERFLOW) // NOLINT
{
    my_cpu -> write (0x0000, 0x24);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 0b01000000);
    my_cpu -> accumulator = 0b01000000;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b01000000);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_OVERFLOW), 1);
}

TEST_F (instructions_logical, BIT_ZPG_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0x24);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 0b10000000);
    my_cpu -> accumulator = 0b10000000;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b10000000);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}