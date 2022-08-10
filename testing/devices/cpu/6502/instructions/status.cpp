//
// Created by lblaga on 11.03.2021.
//

#include "testing/devices/cpu/6502/instructions/status.h"

instructions_status::instructions_status ()
{
    my_bus  = new bus (0x0000, 0XFFFF);
    my_cpu  = new cpu ();
    my_ram  = new memory (0x0000, 0xFFFF);

    my_bus -> add_device (my_cpu);
    my_bus -> add_device (my_ram);
}

instructions_status::~instructions_status ()
{
    delete this -> my_bus;
}


TEST_F (instructions_status, CLC_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x18);
    my_cpu -> set_flag (cpu::F_CARRY, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 0);
}

TEST_F (instructions_status, CLD_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0xD8);
    my_cpu -> set_flag (cpu::F_DECIMAL, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_DECIMAL), 0);
}

TEST_F (instructions_status, CLI_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x58);
    my_cpu -> set_flag (cpu::F_INTERRUPT_DISABLE, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_INTERRUPT_DISABLE), 0);
}

TEST_F (instructions_status, CLV_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0xB8);
    my_cpu -> set_flag (cpu::F_OVERFLOW, 1);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_OVERFLOW), 0);
}


TEST_F (instructions_status, SEC_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x38);
    my_cpu -> set_flag (cpu::F_CARRY, 0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_CARRY), 1);
}

TEST_F (instructions_status, SED_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0xF8);
    my_cpu -> set_flag (cpu::F_DECIMAL, 0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_DECIMAL), 1);
}

TEST_F (instructions_status, SEI_IMP) // NOLINT
{
    my_cpu -> write (0x0000, 0x78);
    my_cpu -> set_flag (cpu::F_INTERRUPT_DISABLE, 0);

    my_cpu -> clock ();
    while (my_cpu -> cycles_left != 0)
        my_cpu -> clock ();

    EXPECT_EQ (my_cpu -> get_flag (cpu::F_INTERRUPT_DISABLE), 1);
}