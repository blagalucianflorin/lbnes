//
// Created by Blaga Lucian-Florin on 07-Mar-21.
//

#include "devices/cpu/6502.h"


uint8_t cpu::IMP () // NOLINT
{
    return (0);
}

uint8_t cpu::ACC ()
{
    return (0);
}

uint8_t cpu::IMM ()
{
    this -> destination_address = this -> program_counter;
    (this -> program_counter)++;

    return (0);
}

uint8_t cpu::ZPG ()
{
    this -> destination_address = this -> read (this -> program_counter);
    (this -> program_counter)++;
    this -> destination_address &= 0x00FF;

    return (0);
}

uint8_t cpu::ZPX ()
{
    this -> destination_address = this -> read (this -> program_counter);
    this -> destination_address += this -> x_register;
    this -> destination_address &= 0x00FF;
    (this -> program_counter)++;

    return (0);
}

uint8_t cpu::ZPY ()
{
    this -> destination_address = this -> read (this -> program_counter);
    this -> destination_address += this -> y_register;
    this -> destination_address &= 0x00FF;
    (this -> program_counter)++;

    return (0);
}

uint8_t cpu::REL ()
{
    this -> jump_relative_address = this -> read (this -> program_counter);
    (this -> program_counter)++;

    if (this -> jump_relative_address & 0x80)
    {
        this -> jump_relative_address |= 0xFF00;
        return (1);
    }

    return (0);
}

uint8_t cpu::ABS ()
{
    this -> destination_address = 0x0000 | this -> read (this -> program_counter++);
    this -> destination_address |= this -> read (this -> program_counter++) << 8;

    return (0);
}

uint8_t cpu::ABX ()
{
    uint16_t aux_1;
    uint16_t aux_2;

    aux_1                       = this -> read (this -> program_counter++);
    aux_2                       = this -> read (this -> program_counter++);
    this -> destination_address = (aux_2 << 8) | aux_1;

    this -> destination_address += this -> x_register;

    if (((this -> destination_address) & 0xFF00) != ((aux_2 << 8) & 0xFF00))
        return (1);

    return (0);
}

uint8_t cpu::ABY ()
{
    uint16_t aux_1;
    uint16_t aux_2;

    aux_1                       = this -> read (this -> program_counter++);
    aux_2                       = this -> read (this -> program_counter++);
    this -> destination_address = (aux_2 << 8) | aux_1;

    this -> destination_address += this -> y_register;

    if (((this -> destination_address) & 0xFF00) != ((aux_2 << 8) & 0xFF00))
        return (1);

    return (0);
}

uint8_t cpu::IND ()
{
    uint8_t  pointer_low_byte;
    uint8_t  pointer_high_byte;
    uint8_t  dest_low_byte;
    uint8_t  dest_high_byte;
    uint16_t pointer;

    pointer_low_byte = this -> read (this -> program_counter);
    (this -> program_counter)++;
    pointer_high_byte  = this -> read (this -> program_counter);

    pointer       = (((uint16_t) pointer_high_byte) << 8) + pointer_low_byte;
    dest_low_byte = this -> read (pointer);
    if (pointer_low_byte == 0xFF)
    {
        pointer        = (((uint16_t) pointer_high_byte) << 8) & 0xFF00;
        dest_high_byte = this -> read (pointer);
    }
    else
        dest_high_byte = this -> read (pointer + 1);

    this -> destination_address = (((uint16_t) dest_high_byte) << 8) + dest_low_byte;

    return (0);
}

uint8_t cpu::IDX ()
{
    uint8_t aux_adr;
    uint8_t aux_1;
    uint8_t aux_2;

    aux_adr = this -> read (this -> program_counter);
    (this -> program_counter)++;

    aux_1 = this -> read ((uint16_t)(aux_adr + ((uint16_t)this -> x_register)) & 0x00FF);
    aux_2 = this -> read ((uint16_t)(aux_adr + ((uint16_t)this -> x_register) + 1) & 0x00FF);

    this -> destination_address = (uint16_t)(aux_2 << 8) | aux_1;

    return (0);
}

uint8_t cpu::IDY ()
{
    uint8_t aux_adr;
    uint8_t aux_1;
    uint8_t aux_2;

    aux_adr = this -> read (this -> program_counter);
    (this -> program_counter)++;

    aux_1 = this -> read (aux_adr & 0x00FF);
    aux_2 = this -> read ((aux_adr + 1) & 0x00FF);

    this -> destination_address = (aux_2 << 8) + aux_1;
    this -> destination_address += this -> y_register;

    if (((this -> destination_address) & 0xFF00) >> 8 != aux_2)
        return (1);

    return (0);
}