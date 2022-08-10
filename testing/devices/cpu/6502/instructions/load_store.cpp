//
// Created by lblaga on 11.03.2021.
//

#include "testing/devices/cpu/6502/instructions/load_store.h"

instructions_load_store::instructions_load_store ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

instructions_load_store::~instructions_load_store ()
{
    delete this -> my_bus;
}


TEST_F (instructions_load_store, LDA_IMM_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0xA9);
    my_cpu -> write (0x0001, 0xAB);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0xAB);
}

TEST_F (instructions_load_store, LDA_IMM_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0xA9);
    my_cpu -> write (0x0001, 0x00);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0x00);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_load_store, LDA_IMM_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xA9);
    my_cpu -> write (0x0001, 0xF0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0xF0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_load_store, LDX_IMM_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0xA2);
    my_cpu -> write (0x0001, 0xAB);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0xAB);
}

TEST_F (instructions_load_store, LDX_IMM_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0xA2);
    my_cpu -> write (0x0001, 0x00);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0x00);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_load_store, LDX_IMM_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xA2);
    my_cpu -> write (0x0001, 0xF0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0xF0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_load_store, LDY_IMM_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0xA0);
    my_cpu -> write (0x0001, 0xAB);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> y_register, 0xAB);
}

TEST_F (instructions_load_store, LDY_IMM_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0xA0);
    my_cpu -> write (0x0001, 0x00);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> y_register, 0x00);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_load_store, LDY_IMM_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xA0);
    my_cpu -> write (0x0001, 0xF0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> y_register, 0xF0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_load_store, STA_ZPG) // NOLINT
{
    my_cpu -> accumulator   = 0xDD;
    my_cpu -> write (0x0000, 0x85);
    my_cpu -> write (0x0001, 0xBB);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock();

    EXPECT_EQ (my_cpu -> read (0x00BB), 0xDD);
}

TEST_F (instructions_load_store, STX_ZPG) // NOLINT
{
    my_cpu -> x_register    = 0xDD;
    my_cpu -> write (0x0000, 0x86);
    my_cpu -> write (0x0001, 0xBB);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock();

    EXPECT_EQ (my_cpu -> read (0x00BB), 0xDD);
}

TEST_F (instructions_load_store, STY_ZPG) // NOLINT
{
    my_cpu -> y_register    = 0xDD;
    my_cpu -> write (0x0000, 0x84);
    my_cpu -> write (0x0001, 0xBB);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock();

    EXPECT_EQ (my_cpu -> read (0x00BB), 0xDD);
}