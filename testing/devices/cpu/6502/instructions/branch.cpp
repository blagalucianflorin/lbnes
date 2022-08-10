//
// Created by lblaga on 11.03.2021.
//

#include "testing/devices/cpu/6502/instructions/branch.h"

instructions_branch::instructions_branch ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

instructions_branch::~instructions_branch ()
{
    delete this -> my_bus;
}


TEST_F (instructions_branch, BCC_REL_CARRY_CLEAR) // NOLINT
{
    my_cpu -> write (0x0000, 0x90);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_CARRY, 0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0003);
}

TEST_F (instructions_branch, BCC_REL_CARRY_SET) // NOLINT
{
    my_cpu -> write (0x0000, 0x90);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_CARRY, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0002);
}


TEST_F (instructions_branch, BCS_REL_CARRY_CLEAR) // NOLINT
{
    my_cpu -> write (0x0000, 0xB0);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_CARRY, 0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0002);
}

TEST_F (instructions_branch, BCS_REL_CARRY_SET) // NOLINT
{
    my_cpu -> write (0x0000, 0xB0);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_CARRY, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0003);
}


TEST_F (instructions_branch, BEQ_REL_ZERO_CLEAR) // NOLINT
{
    my_cpu -> write (0x0000, 0xF0);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_ZERO, 0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0002);
}

TEST_F (instructions_branch, BEQ_REL_ZERO_SET) // NOLINT
{
    my_cpu -> write (0x0000, 0xF0);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_ZERO, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0003);
}


TEST_F (instructions_branch, BMI_REL_NEGATIVE_CLEAR) // NOLINT
{
    my_cpu -> write (0x0000, 0x30);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_NEGATIVE, 0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0002);
}

TEST_F (instructions_branch, BMI_REL_NEGATIVE_SET) // NOLINT
{
    my_cpu -> write (0x0000, 0x30);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_NEGATIVE, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0003);
}


TEST_F (instructions_branch, BNE_REL_ZERO_CLEAR) // NOLINT
{
    my_cpu -> write (0x0000, 0xD0);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_ZERO, 0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0003);
}

TEST_F (instructions_branch, BNE_REL_ZERO_SET) // NOLINT
{
    my_cpu -> write (0x0000, 0xD0);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_ZERO, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0002);
}


TEST_F (instructions_branch, BPL_REL_NEGATIVE_CLEAR) // NOLINT
{
    my_cpu -> write (0x0000, 0x10);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_NEGATIVE, 0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0003);
}

TEST_F (instructions_branch, BPL_REL_NEGATIVE_SET) // NOLINT
{
    my_cpu -> write (0x0000, 0x10);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_NEGATIVE, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0002);
}


TEST_F (instructions_branch, BVC_REL_OVERFLOW_CLEAR) // NOLINT
{
    my_cpu -> write (0x0000, 0x50);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_OVERFLOW, 0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0003);
}

TEST_F (instructions_branch, BVC_REL_OVERFLOW_SET) // NOLINT
{
    my_cpu -> write (0x0000, 0x50);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_OVERFLOW, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0002);
}


TEST_F (instructions_branch, BVS_REL_OVERFLOW_CLEAR) // NOLINT
{
    my_cpu -> write (0x0000, 0x70);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_OVERFLOW, 0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0002);
}

TEST_F (instructions_branch, BVS_REL_OVERFLOW_SET) // NOLINT
{
    my_cpu -> write (0x0000, 0x70);
    my_cpu -> write (0x0001, 0x01);
    my_cpu -> set_flag (cpu::F_OVERFLOW, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> program_counter, 0x0003);
}