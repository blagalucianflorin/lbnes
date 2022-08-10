//
// Created by lblaga on 20.03.2022.
//

#include <iostream>
#include "include/devices/ppu/ppu.h"
#include "debugging/nes_debugger.h"


ppu::ppu (SDL_Renderer *renderer) : device (0x2000, 0x3FFF)
{
    this -> renderer         = renderer;
    this -> nametable_memory = new uint8_t[2048];
    this -> palette_memory   = new uint8_t[33];
    this -> oam              = new uint8_t[0xFF];

    this -> pixels         = new uint32_t[240 * 256 * sizeof (uint32_t)];
    this -> screen_surface = SDL_CreateRGBSurface (0, 256, 240, 24, 0, 0, 0, 0);
    this -> screen         = SDL_CreateTextureFromSurface (this -> renderer, this -> screen_surface);

    for (int i = 0; i < 256 * 240; i++)
        (this -> pixels)[i] = 0x00000000;

    for (int i = 0; i < 2048; i++) (this -> nametable_memory)[i] = 0x00;
    for (int i = 0; i < 33; i++)   (this -> palette_memory)[i] = 0x00;
    for (int i = 0; i < 256; i++)  (this -> oam)[i] = 0x00;

    populate_palette_2C02();
}

ppu::~ppu ()
{
    delete [](this -> nametable_memory);
    delete [](this -> palette_memory);
    delete [](this -> oam);
    delete [](this -> pixels);

    SDL_FreeSurface (this -> screen_surface);
    SDL_DestroyTexture (this -> screen);
}

bool ppu::responds_to (uint16_t address)
{
    return (IS_BETWEEN (this -> lower_bound, address, this -> upper_bound) || address == 0x4014);
}

void    ppu::reset () {}

void    ppu::write (uint16_t address, uint8_t data, bool to_parent_bus) // NOLINT
{
    if (!to_parent_bus)
    {
        address &= 0x3FFF;

        // Pattern
        if (IS_BETWEEN (0x0000, address, 0x1FFF))
            this -> cartridge -> write (address, data);
        // Nametable
        else if (IS_BETWEEN (0x2000, address, 0x3EFF))
        {
            address &= 0x0FFF;

            if (this -> cartridge -> get_mirroring () == cartridge::VERTICAL)
            {
                if (IS_BETWEEN (0x0000, address, 0x03FF) || IS_BETWEEN (0x0800, address, 0x0DFF))
                    (this -> nametable_memory)[address & 0x03FF] = data;
                else if (IS_BETWEEN (0x0400, address, 0x07FF) || IS_BETWEEN (0x0C00, address, 0x0FFF))
                    (this -> nametable_memory)[0x0400 + (address & 0x03FF)] = data;
            }
            else if (this -> cartridge -> get_mirroring () == cartridge::HORIZONTAL)
            {
                if (IS_BETWEEN (0x0000, address, 0x07FF))
                    (this -> nametable_memory)[address & 0x03FF] = data;
                else if (IS_BETWEEN (0x0800, address, 0x0FFF))
                    (this -> nametable_memory)[0x0400 + (address & 0x03FF)] = data;
            }
        }
        // Palettes
        else if (IS_BETWEEN (0x3F00, address, 0x3FFF))
        {
            this -> child_bus -> write (address, data);
        }
        return;
    }

    // OAM
    if (address == 0x4014)
    {
        this -> cpu -> dma (this, data);
    }
    // Registers
    switch ((address - 0x2000) % 8)
    {
        // Control
        case 0x0000:
            this -> control_register              = data;
            this -> temporary_address.nametable_x = this -> get_control_flag (CONTROL_FLAG::F_NAME_TABLE_ADDRESS_ONE);
            this -> temporary_address.nametable_y = this -> get_control_flag (CONTROL_FLAG::F_NAME_TABLE_ADDRESS_TWO);
            break;
        // Mask
        case 0x0001:
            this -> mask_register = data;
            break;
        // Status
        case 0x0002:
            break;
        // OAM Address
        case 0x0003:
            this -> oam_address_register = data;
            break;
        // OAM Data
        case 0x0004:
            (this -> oam)[this -> oam_address_register] = data;
            break;
        // Scroll
        case 0x0005:
            if (!(this -> address_latch))
            {
                this -> fine_x                     = (data & 0b00000111) >> 0;
                this -> temporary_address.coarse_x = (data & 0b11111000) >> 3;
            }
            else
            {
                this -> temporary_address.fine_y   = (data & 0b00000111) >> 0;
                this -> temporary_address.coarse_y = (data & 0b11111000) >> 3;
            }

            this -> address_latch = !(this -> address_latch);
            break;
        // Address
        case 0x0006:
            this -> address_register = data;

            if (!(this -> address_latch))
                this -> temporary_address.data = ((uint16_t) (data & 0x3F) << 8) |
                                                   ((this -> temporary_address.data) & 0x00FF);
            else
            {
                this -> temporary_address.data = ((this -> temporary_address.data & 0xFF00)) | data;
                this -> actual_address.data    = this -> temporary_address.data;
            }
            this -> address_latch = !(this -> address_latch);

            break;
        // Data
        case 0x0007:
            this -> data_register = data;
            this -> write (this -> actual_address.data, data, false);
            this -> actual_address.data += this -> get_control_flag (F_ADDRESS_INCREMENT) ? 32 : 1;
            break;
    }
}

uint8_t ppu::read (uint16_t address, bool from_parent_bus) // NOLINT
{
    if (!from_parent_bus)
    {
        address &= 0x3FFF;

        // Pattern
        if (IS_BETWEEN (0x0000, address, 0x1FFF))
            return (this -> cartridge -> read (address));
        // Nametable
        else if (IS_BETWEEN (0x2000, address, 0x3EFF))
        {
            address &= 0x0FFF;

            if (this -> cartridge -> get_mirroring () == cartridge::VERTICAL)
            {
                if (IS_BETWEEN (0x0000, address, 0x03FF) || IS_BETWEEN (0x0800, address, 0x0DFF))
                    return ((this -> nametable_memory)[address & 0x03FF]);
                else if (IS_BETWEEN (0x0400, address, 0x07FF) || IS_BETWEEN (0x0C00, address, 0x0FFF))
                    return ((this -> nametable_memory)[0x0400 + (address & 0x03FF)]);
            }
            else if (this -> cartridge -> get_mirroring () == cartridge::HORIZONTAL)
            {
                if (IS_BETWEEN (0x0000, address, 0x07FF))
                    return ((this -> nametable_memory)[address & 0x03FF]);
                else if (IS_BETWEEN (0x0800, address, 0x0FFF))
                    return ((this -> nametable_memory)[0x0400 + (address & 0x03FF)]);
            }
        }
        // Palettes
        else if (IS_BETWEEN (0x3F00, address, 0x3FFF))
        {
            return (this -> child_bus -> read (address));
        }
    }

    // OAM
    if (address == 0x4014)
    {
        // Nothing
    }

    uint8_t result = 0x00;
    // Registers
    switch ((address - 0x2000) % 8)
    {
        //   Control      Mask         OAM Address  Scroll       Address
        case 0x0000: case 0x0001: case 0x0003: case 0x0005: case 0x0006:
            return (0x00);
        // Status
        case 0x0002:
            result = ((this -> status_register & 0b11100000) | (this -> previous_data & 0b00011111));

            this -> set_status_flag (STATUS_FLAG::F_VBLANK_STARTED, 0);
            this -> address_latch = false;

            return (result);
        // OAM Data
        case 0x0004:
            return (this -> oam[this -> oam_address_register]);
        // Data
        case 0x0007:
            result                = this -> previous_data;
            this -> previous_data = this -> read (this -> actual_address.data, false);

            if (IS_BETWEEN (0x3F00, this -> actual_address.data, 0x3FFF))
                result = this -> previous_data;

            this -> actual_address.data += (get_control_flag (F_ADDRESS_INCREMENT) == 1 ? 32 : 1);

            return (result);
    }

    return (result);
}

uint8_t ppu::set_control_flag (ppu::CONTROL_FLAG flag, uint8_t value)
{
    value = value ? 1 : 0;

    if (this -> get_control_flag (flag) != value)
        this -> control_register ^= 1 << flag;

    if (flag == F_NMI_ON_VBLANK && value == 1 && this -> scanline >= 241)
        this -> cpu -> interrupt (true);

    return (this -> control_register);
}

uint8_t ppu::get_control_flag (ppu::CONTROL_FLAG flag)
{
    return (((this -> control_register) >> flag) & 1);
}

uint8_t ppu::set_mask_flag (ppu::MASK_FLAG flag, uint8_t value)
{
    value = value ? 1 : 0;

    if (this -> get_mask_flag (flag) != value)
        this -> mask_register ^= 1 << flag;

    return (this -> mask_register);
}

uint8_t ppu::get_mask_flag (ppu::MASK_FLAG flag)
{
    return (((this -> mask_register) >> flag) & 1);
}

uint8_t ppu::set_status_flag (ppu::STATUS_FLAG flag, uint8_t value)
{
    value = value ? 1 : 0;

    if (this -> get_status_flag (flag) != value)
        this -> status_register ^= 1 << flag;

    return (this -> status_register);
}

uint8_t ppu::get_status_flag (ppu::STATUS_FLAG flag)
{
    return (((this -> status_register) >> flag) & 1);
}

uint8_t ppu::get_oam_property (uint8_t entry, ppu::OAM_PROPERTY property)
{
    return ((this -> oam)[(entry * 4) + property]);
}

uint8_t ppu::flip (uint8_t byte)
{
    static const uint8_t table[] = {
        0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
        0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
        0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
        0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
        0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
        0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
        0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
        0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
        0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
        0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
        0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
        0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
        0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
        0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
        0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
        0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
        0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
        0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
        0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
        0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
        0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
        0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
        0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
        0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
        0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
        0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
        0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
        0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
        0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
        0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
        0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
        0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
    };

    return (table[byte]);
}

void ppu::increment_coarse_x ()
{
    if (!(this -> rendering_enabled ()))
        return;

    if (this -> actual_address.coarse_x == 31)
    {
        this -> actual_address.coarse_x    = 0;
        this -> actual_address.nametable_x = ~(this -> actual_address.nametable_x);
    }
    else
        (this -> actual_address.coarse_x)++;
}

void ppu::increment_coarse_fine_y ()
{
    if (!(this -> rendering_enabled ()))
        return;

    if (this -> actual_address.fine_y < 7)
    {
        this -> actual_address.fine_y += 1;
        return;
    }

    this -> actual_address.fine_y = 0;

    if (this -> actual_address.coarse_y == 29)
    {
        this -> actual_address.coarse_y    = 0;
        this -> actual_address.nametable_y = ~(this -> actual_address.nametable_y);
    }
    else if (this -> actual_address.coarse_y == 31)
        this -> actual_address.coarse_y = 0;
    else
        this -> actual_address.coarse_y += 1;
}

void ppu::move_x_data ()
{
    if (!(this -> rendering_enabled ()))
        return;

    this -> actual_address.coarse_x    = this -> temporary_address.coarse_x;
    this -> actual_address.nametable_x = this -> temporary_address.nametable_x;
}

void ppu::move_y_data ()
{
    if (!(this -> rendering_enabled ()))
        return;

    this -> actual_address.coarse_y    = this -> temporary_address.coarse_y;
    this -> actual_address.nametable_y = this -> temporary_address.nametable_y;
    this -> actual_address.fine_y      = this -> temporary_address.fine_y;
}

void ppu::build_shifters ()
{
    this -> background_shift_pattern_low_byte  =
            ((this -> background_shift_pattern_low_byte) & 0xFF00) | this -> next_background_low_byte;
    this -> background_shift_pattern_high_byte =
            ((this -> background_shift_pattern_high_byte) & 0xFF00) | this -> next_background_high_byte;

    this -> background_shift_attribute_low_byte  = ((this -> background_shift_attribute_low_byte) & 0xFF00)
                                                   | ((this -> next_background_attribute & 0b01) ? 0xFF : 0x00);
    this -> background_shift_attribute_high_byte = ((this -> background_shift_attribute_high_byte) & 0xFF00)
                                                   | ((this -> next_background_attribute & 0b10) ? 0xFF : 0x00);
}

void ppu::shift_shifters ()
{
    if (this -> get_mask_flag (MASK_FLAG::F_SHOW_BACKGROUND))
    {
        this -> background_shift_pattern_low_byte    <<= 1;
        this -> background_shift_pattern_high_byte   <<= 1;
        this -> background_shift_attribute_low_byte  <<= 1;
        this -> background_shift_attribute_high_byte <<= 1;
    }

    if (!(this -> get_mask_flag (MASK_FLAG::F_SHOW_SPRITES) && IS_BETWEEN (1, this -> scandot, 257)))
        return;

    for (uint8_t i = 0; i < this -> scanline_sprites_count; i++)
        if (this -> scanline_sprites[i].x > 0)
            this -> scanline_sprites[i].x -= 1;
        else
        {
            (this -> sprite_shift_pattern_low_byte)[i]  <<= 1;
            (this -> sprite_shift_pattern_high_byte)[i] <<= 1;
        }
}

void ppu::clock ()
{
    if (IS_BETWEEN (-1, this -> scanline, 239))
    {
        if (this -> scanline == 0 && this -> scandot == 0 && this -> odd_frame)
            this -> scandot = 1;

        if (this -> scanline == -1 && this -> scandot == 1)
        {
            this -> set_status_flag (STATUS_FLAG::F_VBLANK_STARTED, 0);
            this -> set_status_flag (STATUS_FLAG::F_SPRITE_OVERFLOW, 0);
            this -> set_status_flag (STATUS_FLAG::F_SPRITE_ZERO_HIT, 0);

            for (uint8_t i = 0; i < 8; i++)
            {
                (this -> sprite_shift_pattern_low_byte)[i]  = 0x00;
                (this -> sprite_shift_pattern_high_byte)[i] = 0x00;
            }
        }

        if (IS_BETWEEN (2, this -> scandot, 257) || IS_BETWEEN (321, this -> scandot, 337))
        {
            uint16_t address;

            this -> shift_shifters ();

            switch ((this -> scandot - 1) % 8)
            {
                case 0:
                    this -> build_shifters ();
                    address = 0x2000 | (this -> actual_address.data & 0x0FFF);
                    this -> next_background_chr = this -> read (address, false);
                    break;
                case 2:
                    address = 0x23C0 | this -> actual_address.nametable_y << 11;
                    address |= this -> actual_address.nametable_x << 10;
                    address |= (this -> actual_address.coarse_y >> 2) << 3;
                    address |= this -> actual_address.coarse_x >> 2;

                    this -> next_background_attribute = this -> read (address, false);

                    if (this -> actual_address.coarse_y & 0x02)
                        this -> next_background_attribute >>= 4;
                    if (this -> actual_address.coarse_x & 0x02)
                        this -> next_background_attribute >>= 2;

                    this -> next_background_attribute &= 0b00000011;
                    break;
                case 4:
                    address = (this -> get_control_flag (CONTROL_FLAG::F_BACKGROUND_PATTERN_TABLE)) ? 0x1000 : 0x0000;
                    address += (uint16_t) this -> next_background_chr << 4;
                    address += this -> actual_address.fine_y;
                    this -> next_background_low_byte = this -> read (address, false);
                    break;
                case 6:
                    address = (this -> get_control_flag (CONTROL_FLAG::F_BACKGROUND_PATTERN_TABLE)) ? 0x1000 : 0x0000;
                    address += (uint16_t) this -> next_background_chr << 4;
                    address += this -> actual_address.fine_y;
                    address += 8;
                    this -> next_background_high_byte = this -> read (address, false);
                    break;
                case 7:
                    this -> increment_coarse_x ();
                    break;
            }
        }

        if (this -> scandot == 256)
            this -> increment_coarse_fine_y ();

        if (this -> scandot == 257)
        {
            this -> build_shifters ();
            this -> move_x_data ();
        }

        if (this -> scandot == 338 || this -> scandot == 340)
            this -> next_background_chr = this -> read (0x2000 | (this -> actual_address.data & 0x0FFF), false);

        if (this -> scanline == -1 && IS_BETWEEN (280, this -> scandot, 304))
            this -> move_y_data ();

        // Foreground sprite evaluation
        if (this -> scanline >= 0 && this -> scandot == 257)
        {
            for (uint8_t i = 0; i < 8; i++)
            {
                this -> scanline_sprites[i].y             = 0xFF;
                this -> scanline_sprites[i].chr_id        = 0xFF;
                this -> scanline_sprites[i].attribute     = 0xFF;
                this -> scanline_sprites[i].x             = 0xFF;
                this -> sprite_shift_pattern_low_byte[i]  = 0x00;
                this -> sprite_shift_pattern_high_byte[i] = 0x00;
            }
            this -> scanline_sprites_count = 0x00;
            this -> sprite_zero_included   = false;

            for (uint8_t i = 0; i < 64 && this -> scanline_sprites_count < 9; i++)
            {
                int difference  = (int) this -> scanline - (int) this -> get_oam_property (i, OE_Y);
                int upper_bound = this -> get_control_flag (CONTROL_FLAG::F_SPRITE_SIZE) ? 15 : 7;

                if (IS_BETWEEN (0, difference, upper_bound) && this -> scanline_sprites_count < 8)
                {
                    this -> scanline_sprites[this -> scanline_sprites_count].y         =
                            this -> get_oam_property (i, OE_Y);
                    this -> scanline_sprites[this -> scanline_sprites_count].chr_id    =
                            this -> get_oam_property (i, OE_CHR);
                    this -> scanline_sprites[this -> scanline_sprites_count].attribute =
                            this -> get_oam_property (i, OE_ATTRIBUTE);
                    this -> scanline_sprites[this -> scanline_sprites_count].x         =
                            this -> get_oam_property (i, OE_X);

                    if (i == 0)
                        this -> sprite_zero_included = true;

                    this -> scanline_sprites_count += 1;
                }
            }
            this -> set_status_flag (STATUS_FLAG::F_SPRITE_OVERFLOW, this -> scanline_sprites_count > 8);
        }

        if (this -> scandot == 340)
        {
            for (uint8_t i = 0; i < this -> scanline_sprites_count; i++)
            {
                uint8_t  sprite_pattern_data_low_byte;
                uint8_t  sprite_pattern_data_high_byte;
                uint16_t sprite_pattern_address_low_byte;
                uint16_t sprite_pattern_address_high_byte;

                if (this -> get_control_flag (CONTROL_FLAG::F_SPRITE_SIZE) == 0)
                {
                    if (!(this -> scanline_sprites[i].attribute & 0x80))
                    {
                        sprite_pattern_address_low_byte =
                                (this -> get_control_flag (CONTROL_FLAG::F_SPRITE_PATTERN_TABLE) ? 0x1000 : 0x0000) |
                                (this -> scanline_sprites[i].chr_id << 4) |
                                (this -> scanline - this -> scanline_sprites[i].y);
                    }
                    else
                    {
                        sprite_pattern_address_low_byte =
                                (this -> get_control_flag (CONTROL_FLAG::F_SPRITE_PATTERN_TABLE) ? 0x1000 : 0x0000) |
                                (this -> scanline_sprites[i].chr_id << 4) |
                                (7 - (this -> scanline - this -> scanline_sprites[i].y));
                    }
                }
                else
                {
                    if (!(this -> scanline_sprites[i].attribute & 0x80))
                    {
                        if (this -> scanline - this -> scanline_sprites[i].y < 8)
                        {
                            sprite_pattern_address_low_byte =
                                    (this -> scanline_sprites[i].chr_id ? 0x1000 : 0x000) |
                                    ((this -> scanline_sprites[i].chr_id & 0xEF) << 4) |
                                    ((this -> scanline - (this -> get_oam_property (i, OE_Y))) & 0x07);
                        }
                        else
                        {
                            sprite_pattern_address_low_byte =
                                    (this -> scanline_sprites[i].chr_id ? 0x1000 : 0x000) |
                                    (((this -> scanline_sprites[i].chr_id & 0xEF) + 1) << 4) |
                                    (this -> scanline_sprites[i].y & 0x07);
                        }
                    }
                    else
                    {
                        if (this -> scanline - this -> scanline_sprites[i].y < 8)
                        {
                            sprite_pattern_address_low_byte =
                                    (this -> scanline_sprites[i].chr_id ? 0x1000 : 0x000) |
                                    (((this -> scanline_sprites[i].chr_id & 0xEF) + 1) << 4) |
                                    (7 - (this -> scanline - (this -> scanline_sprites[i].y & 0x07)));
                        }
                        else
                        {
                            sprite_pattern_address_low_byte =
                                    (this -> scanline_sprites[i].chr_id ? 0x1000 : 0x000) |
                                    ((this -> scanline_sprites[i].chr_id & 0xEF) << 4) |
                                    (7 - (this -> scanline - (this -> scanline_sprites[i].y & 0x07)));
                        }
                    }
                }

                sprite_pattern_address_high_byte = sprite_pattern_address_low_byte + 8;

                sprite_pattern_data_low_byte = this -> read (sprite_pattern_address_low_byte, false);
                sprite_pattern_data_high_byte = this -> read (sprite_pattern_address_high_byte, false);

                // flip horizontal
                if (this -> scanline_sprites[i].attribute & 0x40)
                {
                    sprite_pattern_data_low_byte  = this -> flip (sprite_pattern_data_low_byte);
                    sprite_pattern_data_high_byte = this -> flip (sprite_pattern_data_high_byte);
                }

                (this -> sprite_shift_pattern_low_byte)[i]  = sprite_pattern_data_low_byte;
                (this -> sprite_shift_pattern_high_byte)[i] = sprite_pattern_data_high_byte;
            }
        }
    }

    if (this -> scanline == 241 && this -> scandot == 1)
    {
        this -> set_status_flag (STATUS_FLAG::F_VBLANK_STARTED, 1);

        if (this -> get_control_flag (CONTROL_FLAG::F_NMI_ON_VBLANK) == 1)
            this -> cpu -> interrupt (true);
    }

    if (IS_BETWEEN (0, this -> scanline, 239) && IS_BETWEEN (1, this -> scandot, 256))
    {
    // drawing stuff
    uint8_t background_pixel    = 0x00;
    uint8_t background_palette  = 0x00;
    uint8_t foreground_pixel    = 0x00;
    uint8_t foreground_palette  = 0x00;
    uint8_t foreground_priority = 0x00;

    uint8_t final_pixel   = 0x00;
    uint8_t final_palette = 0x00;

    this -> get_background_pixel (background_pixel, background_palette);
    // TODO Check if condition is correct
//    if (this -> get_mask_flag (MASK_FLAG::F_SPRITES_CLIPPING) || this -> scandot >= 9)
        this -> get_foreground_pixel (foreground_pixel, foreground_palette, foreground_priority);

    if (background_pixel == 0x00 && foreground_pixel == 0x00)
    {
        final_pixel   = 0x00;
        final_palette = 0x00;
    }
    else if (background_pixel == 0x00 && foreground_pixel != 0x00)
    {
        final_pixel   = foreground_pixel;
        final_palette = foreground_palette;
    }
    else if (background_pixel != 0x00 && foreground_pixel == 0x00)
    {
        final_pixel   = background_pixel;
        final_palette = background_palette;
    }
    else if (background_pixel != 0x00 && foreground_pixel != 0x00)
    {
        if (this -> rendering_enabled () && this -> sprite_zero_included && this -> drawing_sprite_zero)
        {
            if (this -> get_mask_flag (MASK_FLAG::F_BACKGROUND_CLIPPING) ||
                this -> get_mask_flag (MASK_FLAG::F_SPRITES_CLIPPING))
            {
                if (IS_BETWEEN (1, this -> scandot, 257))
                    this -> set_status_flag (STATUS_FLAG::F_SPRITE_ZERO_HIT, 1);
            }
            else
                if (IS_BETWEEN (9, this -> scandot, 257))
                    this -> set_status_flag (STATUS_FLAG::F_SPRITE_ZERO_HIT, 1);
        }

        if (foreground_priority)
        {
            final_pixel   = foreground_pixel;
            final_palette = foreground_palette;
        }
        else
        {
            final_pixel   = background_pixel;
            final_palette = background_palette;
        }
    }

        rgb_triplet color =
                this -> color_palette [this -> read (0x3F00 + (final_palette << 2) + final_pixel, false) & 0x3F];

        uint32_t sdl_color = 0x00000000 | (color.r << 24) | (color.g << 16) | (color.b << 8) | (0x00 << 0);
        sdl_color >>= 8;
        surface_set_pixel (this -> screen_surface, this -> scandot - 1, this -> scanline, sdl_color);
    }

    (this -> scandot)++;
    if (this -> scandot == 341)
    {
        this -> scandot = 0;
        (this -> scanline)++;

        if (this -> scanline == 261)
        {
            this -> scanline        = -1;
            this -> odd_frame       = !(this -> odd_frame);
            this -> frames_rendered += 1;

            SDL_DestroyTexture (this -> screen);
            this -> screen = SDL_CreateTextureFromSurface (this -> renderer, this -> screen_surface);
            SDL_RenderCopy (this -> renderer, this -> screen, nullptr, nullptr);
        }
    }
}

void ppu::get_background_pixel (uint8_t &pixel, uint8_t &palette)
{
    if (!this -> get_mask_flag (MASK_FLAG::F_SHOW_BACKGROUND))
        return;

    uint8_t pixel_pattern_low_bit  = (this -> background_shift_pattern_low_byte &
                                     (0x8000 >> this -> fine_x)) >> (15 - fine_x);
    uint8_t pixel_pattern_high_bit = (this -> background_shift_pattern_high_byte &
                                     (0x8000 >> this -> fine_x)) >> (15 - fine_x);
    uint8_t pixel_palette_low_bit  = (this -> background_shift_attribute_low_byte &
                                     (0x8000 >> this -> fine_x)) >> (15 - fine_x);
    uint8_t pixel_palette_high_bit = (this -> background_shift_attribute_high_byte &
                                     (0x8000 >> this -> fine_x)) >> (15 - fine_x);

    pixel   = (pixel_pattern_high_bit << 1) | pixel_pattern_low_bit;
    palette = (pixel_palette_high_bit << 1) | pixel_palette_low_bit;
}

void ppu::get_foreground_pixel (uint8_t &pixel, uint8_t &palette, uint8_t &priority)
{
    if (!this -> get_mask_flag (MASK_FLAG::F_SHOW_SPRITES))
        return;

    this -> drawing_sprite_zero = false;

    for (uint8_t i = 0; i < this -> scanline_sprites_count; i++)
        if (this -> scanline_sprites[i].x == 0)
        {
            uint8_t pixel_pattern_low_bit  = ((this -> sprite_shift_pattern_low_byte)[i] & 0x80) >> 7;
            uint8_t pixel_pattern_high_bit = ((this -> sprite_shift_pattern_high_byte)[i] & 0x80) >> 7;

            pixel    = (pixel_pattern_high_bit << 1) | pixel_pattern_low_bit;
            palette  = (this -> scanline_sprites[i].attribute & 0x03) + 0x04;
            priority = (this -> scanline_sprites[i].attribute & 0x20) == 0;

            if (pixel != 0x00)
            {
                if (i == 0)
                    this -> drawing_sprite_zero = true;

                break;
            }
        }
}

void ppu::draw_nametable ()
{
    auto     *nametable             = new uint8_t[1024];
    uint16_t background_chr_address = this -> get_control_flag (F_BACKGROUND_PATTERN_TABLE) ? 0x1000 : 0x0000;
//    uint16_t sprite_chr_address     = this -> get_control_flag (F_SPRITE_PATTERN_TABLE) ? 0x1000 : 0x0000;
    uint16_t nametable_address      = 0x2000 + (0x0400 * (this -> get_control_flag(F_NAME_TABLE_ADDRESS_ONE) + (
                                                         2 * this -> get_control_flag(F_NAME_TABLE_ADDRESS_TWO))));
    uint8_t  chr;


//    if (this -> get_mask_flag (MASK_FLAG::F_SHOW_BACKGROUND))
//    {
        for (int j = 0; j < 2; j++)
        {
            for (uint16_t i = 0; i < 1024; i++)
                nametable[i] = this -> read (nametable_address + (j * 0x0800) + i, false);

            for (uint8_t row = 0; row < 30; row++) {
                for (uint8_t column = 0; column < 32; column++) {
                    chr = nametable[row * 32 + column];

                    this -> draw_tile (background_chr_address + (chr * 16), this -> x_offset + (column * 8),
                                       this -> y_offset + 300 + (row * 8) + (j * 300));
                }
            }
        }
//    }

    if (this -> get_mask_flag (MASK_FLAG::F_SHOW_SPRITES))
    {
        // TODO Draw sprites
    }

    (this -> frames_rendered)++;
}

void ppu::draw_tile (uint16_t chr, int x, int y)
{
    std::map <int, std::vector <int>> aux_palette =
    {
            {0, {0, 0, 0}},
            {1, {255, 0, 0}},
            {2, {0, 255, 0}},
            {3, {0, 0, 255}},
    };

    uint8_t sprite[16];

    for (uint16_t i = 0; i < 16; i++)
        sprite[i] = (this -> cartridge -> character_memory)[chr + i];

    for (uint8_t tile_row = 0; tile_row < 8; tile_row++)
    {
        uint8_t low  = sprite[tile_row];
        uint8_t high = sprite[tile_row + 8];

        for (int tile_pixel = 7; tile_pixel >= 0; tile_pixel--)
        {
            uint8_t color = (high & 1) * 2 + (low & 1);
            low >>= 1;
            high >>= 1;

            SDL_SetRenderDrawColor(renderer, aux_palette[color][0], aux_palette[color][1],
                                   aux_palette[color][2], 255);

            SDL_RenderDrawPoint(renderer, x + tile_pixel, y + tile_row);
        }
    }
}

void ppu::populate_palette_2C02()
{
    (this -> color_palette)[0x00] = {84, 84, 84};    (this -> color_palette)[0x01] = {0, 30, 116};
    (this -> color_palette)[0x02] = {8, 16, 144};    (this -> color_palette)[0x03] = {48, 0, 136};
    (this -> color_palette)[0x04] = {68, 0, 100};    (this -> color_palette)[0x05] = {92, 0, 48};
    (this -> color_palette)[0x06] = {84, 4, 0};      (this -> color_palette)[0x07] = {60, 24, 0};
    (this -> color_palette)[0x08] = {32, 42, 0};     (this -> color_palette)[0x09] = {88, 58, 0};
    (this -> color_palette)[0x0A] = {0, 64, 0};      (this -> color_palette)[0x0B] = {0, 60, 0};
    (this -> color_palette)[0x0C] = {0, 50, 60};     (this -> color_palette)[0x0D] = {0, 0, 0};
    (this -> color_palette)[0x0E] = {0, 0, 0};       (this -> color_palette)[0x0F] = {0, 0, 0};

    (this -> color_palette)[0x10] = {152, 150, 152}; (this -> color_palette)[0x11] = {8, 76, 196};
    (this -> color_palette)[0x12] = {48, 50, 236};   (this -> color_palette)[0x13] = {92, 30, 228};
    (this -> color_palette)[0x14] = {136, 20, 176};  (this -> color_palette)[0x15] = {160, 20, 100};
    (this -> color_palette)[0x16] = {152, 34, 32};   (this -> color_palette)[0x17] = {120, 60, 0};
    (this -> color_palette)[0x18] = {84, 90, 0};     (this -> color_palette)[0x19] = {40, 114, 0};
    (this -> color_palette)[0x1A] = {8, 124, 0};     (this -> color_palette)[0x1B] = {0, 118, 40};
    (this -> color_palette)[0x1C] = {0, 102, 120};   (this -> color_palette)[0x1D] = {0, 0, 0};
    (this -> color_palette)[0x1E] = {0, 0, 0};       (this -> color_palette)[0x1F] = {0, 0, 0};

    (this -> color_palette)[0x20] = {236, 238, 236}; (this -> color_palette)[0x21] = {76, 154, 236};
    (this -> color_palette)[0x22] = {120, 124, 236}; (this -> color_palette)[0x23] = {176, 98, 236};
    (this -> color_palette)[0x24] = {228, 84, 236};  (this -> color_palette)[0x25] = {236, 88, 180};
    (this -> color_palette)[0x26] = {236, 106, 100}; (this -> color_palette)[0x27] = {212, 136, 32};
    (this -> color_palette)[0x28] = {160, 170, 0};   (this -> color_palette)[0x29] = {116, 196, 0};
    (this -> color_palette)[0x2A] = {76, 208, 32};   (this -> color_palette)[0x2B] = {56, 204, 108};
    (this -> color_palette)[0x2C] = {56, 180, 204};  (this -> color_palette)[0x2D] = {60, 60, 60};
    (this -> color_palette)[0x2E] = {0, 0, 0};       (this -> color_palette)[0x2F] = {0, 0, 0};

    (this -> color_palette)[0x30] = {236, 238, 236}; (this -> color_palette)[0x31] = {168, 204, 236};
    (this -> color_palette)[0x32] = {188, 188, 236}; (this -> color_palette)[0x33] = {212, 178, 236};
    (this -> color_palette)[0x34] = {236, 174, 236}; (this -> color_palette)[0x35] = {236, 174, 212};
    (this -> color_palette)[0x36] = {236, 180, 176}; (this -> color_palette)[0x37] = {228, 196, 144};
    (this -> color_palette)[0x38] = {204, 210, 120}; (this -> color_palette)[0x39] = {180, 222, 120};
    (this -> color_palette)[0x3A] = {168, 226, 144}; (this -> color_palette)[0x3B] = {152, 226, 180};
    (this -> color_palette)[0x3C] = {160, 214, 228}; (this -> color_palette)[0x3D] = {160, 162, 160};
    (this -> color_palette)[0x3E] = {0, 0, 0};       (this -> color_palette)[0x3F] = {0, 0, 0};
}
