//
// Created by lblaga on 11.03.2021.
//

#include "testing/devices/cpu/6502/instructions/shifts.h"

instructions_shifts::instructions_shifts ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

instructions_shifts::~instructions_shifts ()
{
    delete this -> my_bus;
}


TEST_F (instructions_shifts, ASL_ACC_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x0A);
    my_cpu -> accumulator = 2;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 4);
}

TEST_F (instructions_shifts, ASL_ACC_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0x0A);
    my_cpu -> accumulator = 0;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_shifts, ASL_ACC_CARRY) // NOLINT
{
    my_cpu -> write (0x0000, 0x0A);
    my_cpu -> accumulator = 0b10000000;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
}

TEST_F (instructions_shifts, ASL_ACC_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0x0A);
    my_cpu -> accumulator = 0b01000000;

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> accumulator, 0b10000000);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_shifts, ASL_ZPG_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x06);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 2);

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> read (0x00CC), 4);
}


TEST_F (instructions_shifts, LSR_ACC_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x4A);
    my_cpu -> accumulator = 4;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 2);
}

TEST_F (instructions_shifts, LSR_ACC_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0x4A);
    my_cpu -> accumulator = 0;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_shifts, LSR_ACC_CARRY) // NOLINT
{
    my_cpu -> write (0x0000, 0x4A);
    my_cpu -> accumulator = 0x01;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
}

TEST_F (instructions_shifts, LSR_ZPG_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x46);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 4);

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> read (0x00CC), 2);
}


TEST_F (instructions_shifts, ROL_ACC_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x2A);
    my_cpu -> accumulator = 2;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 4);
}

TEST_F (instructions_shifts, ROL_ACC_CARRY) // NOLINT
{
    my_cpu -> write (0x0000, 0x2A);
    my_cpu -> accumulator = 0b10000000;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
}

TEST_F (instructions_shifts, ROL_ACC_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0x2A);
    my_cpu -> accumulator = 0;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_shifts, ROL_ACC_CARRY_SET) // NOLINT
{
    my_cpu -> write (0x0000, 0x2A);
    my_cpu -> accumulator = 0;
    my_cpu -> set_flag (cpu::F_CARRY, 1);

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 1);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 0);
}

TEST_F (instructions_shifts, ROL_ACC_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0x2A);
    my_cpu -> accumulator = 0b01000000;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0b10000000);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_shifts, ROL_ZPG_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x26);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 2);
    my_cpu -> accumulator = 2;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> read (0x00CC), 4);
}


TEST_F (instructions_shifts, ROR_ACC_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x6A);
    my_cpu -> accumulator = 4;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 2);
}

TEST_F (instructions_shifts, ROR_ACC_CARRY) // NOLINT
{
    my_cpu -> write (0x0000, 0x6A);
    my_cpu -> accumulator = 1;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
}

TEST_F (instructions_shifts, ROR_ACC_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0x6A);
    my_cpu -> accumulator = 0;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_shifts, ROR_ACC_NEGATIVE_AND_CARRY_SET) // NOLINT
{
    my_cpu -> write (0x0000, 0x6A);
    my_cpu -> accumulator = 0;
    my_cpu -> set_flag (cpu::F_CARRY, 1);

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0b10000000);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 0);
}


TEST_F (instructions_shifts, ROR_ZPG_NORMAL) // NOLINT
{
    my_cpu -> write (0x0000, 0x66);
    my_cpu -> write (0x0001, 0xCC);
    my_cpu -> write (0x00CC, 4);

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> read (0x00CC), 2);
}