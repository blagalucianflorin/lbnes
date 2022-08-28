//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#ifndef NEMULATOR_CPU_H
#define NEMULATOR_CPU_H

#include "include/forwards/classes.h"
#include "include/devices/device.h"
#include "include/bus/bus.h"
#include "include/devices/ppu/ppu.h"


class cpu : public device
{
#ifdef G_TESTING
public:
#else
private:
#endif
    bool     accumulator_addressing = false;
    uint16_t jump_relative_address  = 0xFFFF;
    uint16_t destination_address    = 0xFFFE;

    // Registers
    uint8_t  flags_register  = 0x34;
    uint8_t  accumulator     = 0x00;
    uint8_t  x_register      = 0x00;
    uint8_t  y_register      = 0x00;
    uint16_t program_counter = 0x0000;
    uint8_t  stack_pointer   = 0xFD;

    // Record cycles left for current operation
    long     cycles_left    = 0;
    long     cycles_elapsed = 0;

    // OAM DMA
    bool      dma_active = false;
    bool      dma_begin  = false;
    uint8_t   dma_entry  = 0x00;
    uint8_t   dma_data   = 0x00;
    class ppu *dma_ppu   = nullptr;
    uint8_t   dma_page   = 0x00;

    // Operations
    struct  operation
    {
        uint8_t (cpu::*instruction) ()     = &cpu::UNK;
        uint8_t (cpu::*addressing_mode) () = &cpu::IMP;
        uint8_t cycles_required            = 0;
    }       operations[256];
    uint8_t opcode;

    enum FLAG
    {
        F_CARRY             = 0,
        F_ZERO              = 1,
        F_INTERRUPT_DISABLE = 2,
        F_DECIMAL           = 3,
        F_BREAK             = 4,
        F_UNUSED            = 5,
        F_OVERFLOW          = 6,
        F_NEGATIVE          = 7
    };

    uint8_t set_flag (FLAG flag, uint8_t value);

    uint8_t get_flag (FLAG flag) const;

    void    push_to_stack (uint8_t value);

    uint8_t pull_from_stack ();

    uint8_t get_input_data ();

    void    populate_operations ();


    // Addressing modes
    uint8_t IMP (); uint8_t ACC (); uint8_t IMM (); uint8_t ZPG ();
    uint8_t ZPX (); uint8_t ZPY (); uint8_t REL (); uint8_t ABS ();
    uint8_t ABX (); uint8_t ABY (); uint8_t IND (); uint8_t IDX ();
    uint8_t IDY ();


    // Instructions
    uint8_t ADC (); uint8_t AND (); uint8_t ASL (); uint8_t BCC ();
    uint8_t BCS (); uint8_t BEQ (); uint8_t BIT (); uint8_t BMI ();
    uint8_t BNE (); uint8_t BPL (); uint8_t BRK (); uint8_t BVC ();
    uint8_t BVS (); uint8_t CLC (); uint8_t CLD (); uint8_t CLI ();
    uint8_t CLV (); uint8_t CMP (); uint8_t CPX (); uint8_t CPY ();
    uint8_t DEC (); uint8_t DEX (); uint8_t DEY (); uint8_t EOR ();
    uint8_t INC (); uint8_t INX (); uint8_t INY (); uint8_t JMP ();
    uint8_t JSR (); uint8_t LDA (); uint8_t LDX (); uint8_t LDY ();
    uint8_t LSR (); uint8_t NOP (); uint8_t ORA (); uint8_t PHA ();
    uint8_t PHP (); uint8_t PLA (); uint8_t PLP (); uint8_t ROL ();
    uint8_t ROR (); uint8_t RTI (); uint8_t RTS (); uint8_t SBC ();
    uint8_t SEC (); uint8_t SED (); uint8_t SEI (); uint8_t STA ();
    uint8_t STX (); uint8_t STY (); uint8_t TAX (); uint8_t TAY ();
    uint8_t TSX (); uint8_t TXA (); uint8_t TXS (); uint8_t TYA ();
    uint8_t UNK (); // Unknown instruction

    // Illegal instructions
    uint8_t DOP (); uint8_t TOP (); uint8_t LAX (); uint8_t SAX ();
    uint8_t DCP (); uint8_t ISC (); uint8_t SLO (); uint8_t RLA ();
    uint8_t SRE (); uint8_t RRA (); uint8_t ANC (); uint8_t ASR ();
    uint8_t ARR (); uint8_t ANE (); uint8_t TAS (); uint8_t LAS ();
    uint8_t SBX (); uint8_t SHY (); uint8_t SHX (); uint8_t SHA ();
    uint8_t LXA ();

public:
    cpu ();

    inline bool responds_to (uint16_t address) override { return (false); }

    void    write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t read (uint16_t address, bool from_parent_bus = true) override; // NOLINT

    void    reset ();

    void    clock ();

    void    step ();

    void    interrupt (bool force);

    inline void wait (long cycles) { this -> cycles_left += cycles; }

    inline long get_cycles_elapsed () { return (this -> cycles_elapsed); }

    void    dma (ppu *target_ppu, uint8_t page);
};

#endif //NEMULATOR_CPU_H
