//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#include "include/devices/cpu/6502.h"


cpu::cpu () : device (0x0000, 0x0000) // NOLINT
{
    this -> is_mem = false;
    this -> populate_operations ();
}

void cpu::write (uint16_t address, uint8_t data, bool to_parent_bus) // NOLINT
{
    (this -> parent_bus) -> write (address, data);
}

uint8_t cpu::read (uint16_t address, bool from_parent_bus) // NOLINT
{
    return ((this -> parent_bus) -> read (address));
}

uint8_t cpu::set_flag (cpu::FLAG flag, uint8_t value)
{
    value = value ? 1 : 0;

    if (this -> get_flag (flag) != value)
        this -> flags_register ^= 1 << flag;

    return (this -> flags_register);
}

uint8_t cpu::get_flag (cpu::FLAG flag) const
{
    return (((this -> flags_register) >> flag) & 1);
}

void cpu::reset ()
{
    this -> flags_register         = 0x34;
    this -> accumulator            = 0x00;
    this -> x_register             = 0x00;
    this -> y_register             = 0x00;
    this -> program_counter        = (this -> read (0xFFFD) << 8) | (this -> read (0xFFFC));
    this -> stack_pointer          = 0xFD;

    this -> cycles_left            = 0;
    this -> cycles_elapsed         = 8;
    this -> accumulator_addressing = false;
    this -> jump_relative_address  = 0xFFFF;
    this -> destination_address    = 0xFFFE;
}

[[maybe_unused]] void nop (){} // NOLINT

void cpu::clock ()
{
    if (this -> dma_active)
    {
        if (!(this -> dma_begin))
        {
            if (this -> cycles_elapsed % 2 == 1)
                this -> dma_begin = true;
        }
        else
        {
            if (this -> cycles_elapsed % 2 == 0)
                this -> dma_data = this -> read ((this -> dma_page << 8) | (this -> dma_entry));
            else
                (this -> dma_ppu -> dma ())[this -> dma_entry++] = this -> dma_data;

            if (this -> dma_entry == 0xFF)
            {
                (this -> dma_ppu -> dma ())[this -> dma_entry] =
                        this -> read ((this -> dma_page << 8) | (this -> dma_entry));
                this -> dma_begin  = false;
                this -> dma_active = false;
            }
        }
    }
    else if (this -> cycles_left == 0)
    {
        this -> set_flag (cpu::F_UNUSED, 1);
        this -> opcode = this -> read (this -> program_counter);

        (this -> program_counter)++;

        this -> cycles_left += (this -> operations[this -> opcode].cycles_required);
        this -> cycles_left += (this ->* operations[this -> opcode].addressing_mode)();

        this -> accumulator_addressing = this -> operations[this -> opcode].addressing_mode == &cpu::ACC;

        this -> cycles_left += (this ->* operations[this -> opcode].instruction)();

        this -> set_flag (cpu::F_UNUSED, 1);
    }

    if (this -> cycles_left != 0)
    {
        (this -> cycles_left)--;
        (this -> cycles_elapsed)++;
    }
}

uint8_t cpu::get_input_data ()
{
    static uint8_t input_data;

    if (this -> accumulator_addressing)
        input_data = this -> accumulator;
    else
        input_data = this -> read (this -> destination_address);

    return (input_data);
}

void cpu::push_to_stack(uint8_t value)
{
    this -> write (0x0100 + (this -> stack_pointer), value);
    this -> stack_pointer   -= 1;
}

uint8_t cpu::pull_from_stack ()
{
    this -> stack_pointer   += 1;

    return (this -> read (0x0100 + (stack_pointer)));
}

void cpu::step ()
{
    if (this -> cycles_left != 0)
    {
        while (this -> cycles_left != 0)
            this -> clock ();
    }
    else
    {
        this -> clock ();

        while (this -> cycles_left != 0)
            this -> clock ();
    }
}

void cpu::interrupt (bool force)
{
    if (!force && !(this -> get_flag (cpu::F_INTERRUPT_DISABLE)))
        return;

    this -> push_to_stack ((this -> program_counter & 0xFF00) >> 8);
    this -> push_to_stack (this -> program_counter & 0x00FF);

    this -> set_flag (cpu::F_INTERRUPT_DISABLE, 1);
    this -> set_flag (cpu::F_BREAK, 0);
    this -> set_flag (cpu::F_UNUSED, 1);
    this -> push_to_stack (this -> flags_register);

    this -> destination_address = (force ? 0xFFFA : 0xFFFE);
    this -> program_counter = this -> parent_bus -> read (this -> destination_address) |
                             (this -> parent_bus -> read (this -> destination_address + 1) << 8);

    this -> cycles_left = (force ? 8 : 7);
}

void cpu::dma (ppu *target_ppu, uint8_t page)
{
    this -> dma_ppu     = target_ppu;
    this -> dma_page    = page;
    this -> dma_active  = true;
    this -> dma_entry   = 0x00;
    this -> cycles_left = 512;
}
