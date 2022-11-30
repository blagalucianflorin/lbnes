//
// Created by lblaga on 20.03.2022.
//

#ifndef NEMULATOR_PPU_H
#define NEMULATOR_PPU_H

#include "forwards/classes.h"
#include "devices/device.h"
#include "devices/ppu/exceptions/ppu_exception.h"
#include "bus/bus.h"
#include "devices/cartridges/cartridge.h"
#include "misc/macros.h"
#include "devices/cpu/6502.h"
#include "devices/ppu/oam.h"

#include <SDL.h>

#include <map>
#include <array>


class ppu : public device, public std::enable_shared_from_this <ppu>
{
private:
    std::array <uint8_t, 2048> nametable_memory {};
    std::array <uint8_t, 33>   palette_memory {};
    std::array <uint8_t, 256>  oam {};

    uint8_t control_register     = 0x0000;
    uint8_t mask_register        = 0x0000;
    uint8_t status_register      = 0x0000;
    uint8_t oam_address_register = 0x0000;
    uint8_t oam_data_register    = 0x0000;
    uint8_t scroll_register      = 0x0000;
    uint8_t address_register     = 0x0000;
    uint8_t data_register        = 0x0000;

    uint8_t oam_dma_register     = 0x0000;

    /*
     * Determines which byte of destination_address will be written next
     * false: high byte
     * true:  low  byte
     */
    bool address_latch = false;

    enum CONTROL_FLAG
    {
        F_NAME_TABLE_ADDRESS_ONE   = 0,
        F_NAME_TABLE_ADDRESS_TWO   = 1,
        F_ADDRESS_INCREMENT        = 2,
        F_SPRITE_PATTERN_TABLE     = 3,
        F_BACKGROUND_PATTERN_TABLE = 4,
        F_SPRITE_SIZE              = 5,
        F_PPU_SELECT               = 6, // Unused
        F_NMI_ON_VBLANK            = 7
    };

    enum MASK_FLAG
    {
        F_GREYSCALE              = 0,
        F_BACKGROUND_CLIPPING    = 1,
        F_SPRITES_CLIPPING       = 2,
        F_SHOW_SPRITES           = 4,
        F_SHOW_BACKGROUND        = 3,
        F_BACKGROUND_COLOR_ONE   = 5,
        F_BACKGROUND_COLOR_TWO   = 6,
        F_BACKGROUND_COLOR_THREE = 7,
    };

    enum STATUS_FLAG
    {
        F_LAST_WRITE_BIT_ONE   = 0,
        F_LAST_WRITE_BIT_TWO   = 1,
        F_LAST_WRITE_BIT_THREE = 2,
        F_LAST_WRITE_BIT_FOUR  = 3,
        F_LAST_WRITE_BIT_FIVE  = 4,
        F_SPRITE_OVERFLOW      = 5,
        F_SPRITE_ZERO_HIT      = 6,
        F_VBLANK_STARTED       = 7
    };

public:
    struct rgb_triplet
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    std::array <rgb_triplet, 64> color_palette {};

private:
    enum OAM_PROPERTY
    {
        OE_Y         = 0,
        OE_CHR       = 1,
        OE_ATTRIBUTE = 2,
        OE_X         = 3,
    };

    struct oam_entry
    {
        uint8_t y;
        uint8_t chr_id;
        uint8_t attribute;
        uint8_t x;
    };

    struct loopy_fields
    {
        uint16_t coarse_x    : 5;
        uint16_t coarse_y    : 5;
        uint16_t nametable_x : 1;
        uint16_t nametable_y : 1;
        uint16_t fine_y      : 3;
        uint16_t unused      : 1;
    };

    union loopy
    {
        loopy_fields fields;
        uint16_t     data;
    };

    loopy temporary_address {};
    loopy actual_address {};

    uint8_t fine_x {};

    int  scanline  = 0;
    int  scandot   = 0;
    bool odd_frame = false;

    uint8_t previous_data = 0x00;

    // Rendering
    SDL_Renderer *renderer       = nullptr;
    SDL_Texture  *screen         = nullptr;
    SDL_Surface  *screen_surface = nullptr;

    std::array <uint32_t, 240 * 256> pixels {};
    std::array <uint8_t, 240 * 256>  pixels_small {};

    // Directly attached devices
    std::shared_ptr <class cpu>       cpu;
    std::shared_ptr <class cartridge> cartridge;

    void  populate_palette_2C02 ();

    uint8_t next_background_low_byte  = 0x00;
    uint8_t next_background_high_byte = 0x00;
    uint8_t next_background_attribute = 0x00;
    uint8_t next_background_chr       = 0x00;

    inline bool rendering_enabled ()
    {
        return (this -> get_mask_flag (MASK_FLAG::F_SHOW_BACKGROUND)
                || this -> get_mask_flag (MASK_FLAG::F_SHOW_SPRITES));
    }

    void increment_coarse_x ();
    void increment_coarse_fine_y ();
    void move_x_data ();
    void move_y_data ();

    uint16_t background_shift_pattern_low_byte    = 0x0000;
    uint16_t background_shift_pattern_high_byte   = 0x0000;
    uint16_t background_shift_attribute_low_byte  = 0x0000;
    uint16_t background_shift_attribute_high_byte = 0x0000;

    void build_shifters ();
    void shift_shifters ();

    void get_background_pixel (uint8_t &pixel, uint8_t &palette) const;

    uint8_t                   scanline_sprites_count = 0x00;
    std::array <oam_entry, 8> scanline_sprites {};

    uint8_t  get_oam_property (uint8_t entry, OAM_PROPERTY property);

    std::array <uint8_t, 8> sprite_shift_pattern_low_byte {};
    std::array <uint8_t, 8> sprite_shift_pattern_high_byte {};

    static uint8_t flip (uint8_t byte);

    void get_foreground_pixel (uint8_t &pixel, uint8_t &palette, uint8_t &priority);

    bool sprite_zero_included = false;
    bool drawing_sprite_zero  = false;

    cartridge::MIRRORING_TYPE mirroring_type = cartridge::HORIZONTAL;

public:
    ppu () : device (0x2000, 0x3FFF) {}

    explicit ppu (SDL_Renderer *renderer);

    ~ppu () override;

    void     clock ();

    void     reset ();

    bool     responds_to (uint16_t address) override;

    void     write (uint16_t address, uint8_t data, bool to_parent_bus = true) override; // NOLINT

    uint8_t  read (uint16_t address, bool from_parent_bus = true) override; // NOLINT

    [[maybe_unused]] uint8_t  set_control_flag (CONTROL_FLAG flag, uint8_t value);

    uint8_t  get_control_flag (CONTROL_FLAG flag) const;

    [[maybe_unused]] uint8_t  set_mask_flag (MASK_FLAG flag, uint8_t value);

    uint8_t  get_mask_flag (MASK_FLAG flag) const;

    uint8_t  set_status_flag (STATUS_FLAG flag, uint8_t value);

    uint8_t  get_status_flag (STATUS_FLAG flag) const;

    inline const std::array <uint32_t, 240 * 256> &get_pixels () { return (this -> pixels); }

    inline std::array <uint8_t, 240 * 256>  &get_pixels_small () { return (this -> pixels_small); }

    // DMA
    inline std::array <uint8_t, 256> &dma () { return (this -> oam); }

    // Attach devices for direct access
    inline void attach (std::shared_ptr <class cpu> cpu) { this -> cpu = cpu; }

    inline void attach (SDL_Renderer *new_renderer) { this -> renderer = new_renderer; }

    inline void attach (std::shared_ptr <class cartridge> cartridge)
    {
        this -> cartridge       = cartridge;
        this -> mirroring_type  = this -> cartridge -> get_mirroring ();
    }

    inline bool is_odd_frame () { return (this -> odd_frame); }

    long long frames_rendered = 0;
};

#endif //NEMULATOR_PPU_H
