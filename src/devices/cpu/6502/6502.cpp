//
// Created by Blaga Lucian-Florin on 05-Mar-21.
//

#include "devices/cpu/6502.h"


cpu::cpu () : device (0x0000, 0x0000) // NOLINT
{
    this -> is_mem = false;
    this -> populate_operations ();
}

void cpu::write (uint16_t address, uint8_t data, bool /*to_parent_bus*/) // NOLINT
{
    (this -> parent_bus) -> write (address, data);
}

uint8_t cpu::read (uint16_t address, bool /*from_parent_bus*/) // NOLINT
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


void cpu::clock () noexcept
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

std::string cpu::save_state (const std::string &name)
{
    YAML::Node base_device_node = YAML::Load (device::save_state ("base_device"));
    YAML::Node final_node;

    final_node[name] = YAML::Node ();

    final_node[name]["accumulator_addressing"] = this -> accumulator_addressing;
    final_node[name]["jump_relative_address"]  = this -> jump_relative_address;
    final_node[name]["destination_address"]    = this -> destination_address;

    final_node[name]["flags_register"]  = this -> flags_register;
    final_node[name]["accumulator"]     = this -> accumulator;
    final_node[name]["x_register"]      = this -> x_register;
    final_node[name]["y_register"]      = this -> y_register;
    final_node[name]["program_counter"] = this -> program_counter;
    final_node[name]["stack_pointer"]   = this -> stack_pointer;

    final_node[name]["cycles_left"]    = this -> cycles_left;
    final_node[name]["cycles_elapsed"] = this -> cycles_elapsed;

    final_node[name]["dma_active"] = this -> dma_active;
    final_node[name]["dma_begin"]  = this -> dma_begin;
    final_node[name]["dma_entry"]  = this -> dma_entry;
    final_node[name]["dma_data"]   = this -> dma_data;
    final_node[name]["dma_page"]   = this -> dma_page;

    for (auto pair : base_device_node["base_device"])
        final_node[name]["base_device"][pair.first] = pair.second;

    return (YAML::Dump(final_node));
}

void cpu::load_state (std::string saved_state)
{
    YAML::Node saved_node = YAML::Load (saved_state.c_str ()).begin() -> second;
    device::load_state (YAML::Dump(saved_node["base_device"]));

    this -> accumulator_addressing = saved_node["accumulator_addressing"].as<bool>();
    this -> jump_relative_address  = saved_node["jump_relative_address"].as<uint16_t>();
    this -> destination_address    = saved_node["destination_address"].as<uint16_t>();

    this -> flags_register  = saved_node["flags_register"].as<uint8_t>();
    this -> accumulator     = saved_node["accumulator"].as<uint8_t>();
    this -> x_register      = saved_node["x_register"].as<uint8_t>();
    this -> y_register      = saved_node["y_register"].as<uint8_t>();
    this -> program_counter = saved_node["program_counter"].as<uint16_t>();
    this -> stack_pointer   = saved_node["stack_pointer"].as<uint8_t>();

    this -> cycles_left    = saved_node["cycles_left"].as<long>();
    this -> cycles_elapsed = saved_node["cycles_elapsed"].as<long>();

    this -> dma_active = saved_node["dma_active"].as<bool>();
    this -> dma_begin  = saved_node["dma_begin"].as<bool>();
    this -> dma_entry  = saved_node["dma_entry"].as<uint8_t>();
    this -> dma_data   = saved_node["dma_data"].as<uint8_t>();
    this -> dma_page   = saved_node["dma_page"].as<uint8_t>();
}
