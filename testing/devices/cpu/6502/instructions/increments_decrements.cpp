//
// Created by lblaga on 11.03.2021.
//

#include "testing/devices/cpu/6502/instructions/increments_decrements.h"

instructions_increments_decrements::instructions_increments_decrements ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

instructions_increments_decrements::~instructions_increments_decrements ()
{
    delete this -> my_bus;
}


TEST_F (instructions_increments_decrements, INC_ZPG_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0xE6);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 0x06);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> read (0x00CC), 0x07);
}

TEST_F (instructions_increments_decrements, INC_ZPG_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0xE6);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 0xFF);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> read (0x00CC), 0x00);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_increments_decrements, INC_ZPG_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xE6);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 0xF1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> read (0x00CC), 0xF2);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_increments_decrements, INX_IMP_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0xE8);
    my_cpu -> x_register = 0x06;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0x07);
}

TEST_F (instructions_increments_decrements, INX_IMP_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0xE8);
    my_cpu -> x_register = 0xFF;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0x00);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_increments_decrements, INX_IMP_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xE8);
    my_cpu -> x_register = 0xF0;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0xF1);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_increments_decrements, INY_IMP_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0xC8);
    my_cpu -> y_register = 0x06;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> y_register, 0x07);

}

TEST_F (instructions_increments_decrements, INY_IMP_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0xC8);
    my_cpu -> y_register = 0xFF;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> y_register, 0x00);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_increments_decrements, INY_IMP_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xC8);
    my_cpu -> y_register = 0xF0;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> y_register, 0xF1);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_increments_decrements, DEC_ZPG_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0xC6);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 0x06);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> read (0x00CC), 0x05);
}

TEST_F (instructions_increments_decrements, DEC_ZPG_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0xC6);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 0x01);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> read (0x00CC), 0x00);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_increments_decrements, DEC_ZPG_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xC6);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 0x00);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> read (0x00CC), 0xFF);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_increments_decrements, DEX_IMP_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0xCA);
    my_cpu -> x_register = 0x06;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0x05);
}

TEST_F (instructions_increments_decrements, DEX_IMP_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0xCA);
    my_cpu -> x_register = 0x01;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0x00);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_increments_decrements, DEX_IMP_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xCA);
    my_cpu -> x_register = 0x00;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> x_register, 0xFF);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_increments_decrements, DEY_IMP_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x88);
    my_cpu -> y_register = 0x06;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> y_register, 0x05);
}

TEST_F (instructions_increments_decrements, DEY_IMP_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0x88);
    my_cpu -> y_register = 0x01;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> y_register, 0x00);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_increments_decrements, DEY_IMP_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0x88);
    my_cpu -> y_register = 0x00;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> y_register, 0xFF);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}
