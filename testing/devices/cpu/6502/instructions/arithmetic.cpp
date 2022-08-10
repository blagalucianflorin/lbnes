//
// Created by lblaga on 11.03.2021.
//

#include "testing/devices/cpu/6502/instructions/arithmetic.h"

instructions_arithmetic::instructions_arithmetic ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

instructions_arithmetic::~instructions_arithmetic ()
{
    delete this -> my_bus;
}


TEST_F (instructions_arithmetic, AND_IMM) // NOLINT
{
    my_cpu -> write (0x0000, 0x29);
    my_cpu -> write (0x0001, 0xFF);
    my_cpu -> accumulator = 0xF0;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0xF0);
}


TEST_F (instructions_arithmetic, ADC_IMM_OVERFLOW) // NOLINT
{
    my_cpu -> write (0x0000, 0x69);
    my_cpu -> write (0x00001, 0x0F);
    my_cpu -> accumulator = 0xF0;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0xFF);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_OVERFLOW), 0);
}

TEST_F (instructions_arithmetic, ADC_IMM_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0x69);
    my_cpu -> write (0x0001, 0x00);
    my_cpu -> accumulator = 0x00;
    my_cpu -> set_flag (cpu::F_CARRY, 0);

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0x00);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}


TEST_F (instructions_arithmetic, SBC_IMM_NORMAL_CARRY_SET) // NOLINT
{
    my_cpu -> write (0x0000, 0xE9);
    my_cpu -> write (0x0001, 12);
    my_cpu -> accumulator = 35;
    my_cpu -> set_flag (cpu::F_CARRY, 1);

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 23);
}


TEST_F (instructions_arithmetic, SBC_IMM_NORMAL_CARRY_CLEAR) // NOLINT
{
    my_cpu -> write (0x0000, 0xE9);
    my_cpu -> write (0x0001, 12);
    my_cpu -> accumulator = 35;
    my_cpu -> set_flag (cpu::F_CARRY, 0);

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 22);
}

TEST_F (instructions_arithmetic, SBC_IMM_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xE9);
    my_cpu -> write (0x0001, 1);
    my_cpu -> accumulator = 0;
    my_cpu -> set_flag (cpu::F_CARRY, 1);

    my_cpu -> step ();

    EXPECT_EQ ((int8_t) my_cpu -> accumulator, -1);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}

TEST_F (instructions_arithmetic, SBC_IMM_ZERO) // NOLINT
{
    my_cpu -> write (0x0000, 0xE9);
    my_cpu -> write (0x0001, 1);
    my_cpu -> accumulator = 1;
    my_cpu -> set_flag (cpu::F_CARRY, 1);

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> accumulator, 0);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}


TEST_F (instructions_arithmetic, CMP_IMM_ACCUMULATOR_BIGGER) // NOLINT
{
    my_cpu -> write (0x0000, 0xC9);
    my_cpu -> write (0x0001, 0x0F);
    my_cpu -> accumulator = 0xF0;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
}

TEST_F (instructions_arithmetic, CMP_IMM_ACCUMULATOR_EQUAL) // NOLINT
{
    my_cpu -> write (0x0000, 0xC9);
    my_cpu -> write (0x0001, 0x0F);
    my_cpu -> accumulator = 0x0F;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_arithmetic, CMP_IMM_RESULT_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xC9);
    my_cpu -> write (0x0001, 0x10);
    my_cpu -> accumulator = 0x0F;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_arithmetic, CPX_IMM_X_BIGGER) // NOLINT
{
    my_cpu -> write (0x0000, 0xE0);
    my_cpu -> write (0x0001, 0x0F);
    my_cpu -> x_register    = 0xF0;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
}

TEST_F (instructions_arithmetic, CPX_IMM_X_EQUAL) // NOLINT
{
    my_cpu -> write (0x0000, 0xE0);
    my_cpu -> write (0x0001, 0x0F);
    my_cpu -> x_register    = 0x0F;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_arithmetic, CPX_IMM_X_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xE0);
    my_cpu -> write (0x0001, 0x10);
    my_cpu -> x_register    = 0x0F;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}


TEST_F (instructions_arithmetic, CPY_IMM_Y_BIGGER) // NOLINT
{
    my_cpu -> write (0x0000, 0xC0);
    my_cpu -> write (0x0001, 0x0F);
    my_cpu -> y_register    = 0xF0;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
}

TEST_F (instructions_arithmetic, CPY_IMM_Y_EQUAL) // NOLINT
{
    my_cpu -> write (0x0000, 0xC0);
    my_cpu -> write (0x0001, 0x0F);
    my_cpu -> y_register    = 0x0F;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
    EXPECT_EQ (my_cpu -> get_flag (cpu::F_ZERO), 1);
}

TEST_F (instructions_arithmetic, CPY_IMM_Y_NEGATIVE) // NOLINT
{
    my_cpu -> write (0x0000, 0xC0);
    my_cpu -> write (0x0001, 0x10);
    my_cpu -> y_register    = 0x0F;

    my_cpu -> step ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_NEGATIVE), 1);
}