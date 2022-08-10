//
// Created by lblaga on 20.03.2022.
//

#include "testing/devices/cartridges/cartridge.h"

test_cartridge::test_cartridge ()
{
    this -> my_cartridge = nullptr;
}

test_cartridge::~test_cartridge ()
{
    delete this -> my_cartridge;
}


TEST_F (test_cartridge, CARTRIDGE_UNSUPPORTED_FORMAT) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "unsupported.format");
    }
    catch (cartridge_exception &e)
    {
        EXPECT_EQ (std::string ("Unsupported file format: .format"), std::string (e.what ()));
    }
}

TEST_F (test_cartridge, INES_CORRUPTED_HEADER_BEGINNING) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "corrupted_header_beginning.ines");
    }
    catch (cartridge_exception &e)
    {
        EXPECT_EQ (std::string ("iNES: Corrupted file header"), std::string (e.what ()));
    }
}

TEST_F (test_cartridge, INES_CORRUPTED_HEADER_ENDING) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "corrupted_header_ending.ines");
    }
    catch (cartridge_exception &e)
    {
        EXPECT_EQ (std::string ("iNES: Corrupted file header"), std::string (e.what ()));
    }
}

TEST_F (test_cartridge, INES_UNSUPPORTED_VERSION) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "ines_two.ines");
    }
    catch (cartridge_exception &e)
    {
        EXPECT_EQ (std::string ("iNES: Only iNES 1.0 is supported"), std::string (e.what ()));
    }
}

TEST_F (test_cartridge, INES_MIRRORING_FOUR_SCREEN) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "mirroring_four_screen.ines");
    }
    catch (cartridge_exception &e)
    {
        FAIL () << std::string ("Exception caught: ") + std::string (e.what ());
    }
    EXPECT_EQ (this -> my_cartridge -> mirroring_type, cartridge::FOUR_SCREEN);
}

TEST_F (test_cartridge, INES_MIRRORING_HORIZONTAL) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "mirroring_horizontal.ines");
    }
    catch (cartridge_exception &e)
    {
        FAIL () << std::string ("Exception caught: ") + std::string (e.what ());
    }
    EXPECT_EQ (this -> my_cartridge -> mirroring_type, cartridge::HORIZONTAL);
}

TEST_F (test_cartridge, INES_MIRRORING_VERTICAL) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "mirroring_vertical.ines");
    }
    catch (cartridge_exception &e)
    {
        FAIL () << std::string ("Exception caught: ") + std::string (e.what ());
    }
    EXPECT_EQ (this -> my_cartridge -> mirroring_type, cartridge::VERTICAL);
}

TEST_F (test_cartridge, INES_PROGRAM_CONTENT_BEGINNING) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "program_content_beginning.ines");
    }
    catch (cartridge_exception &e)
    {
        FAIL () << std::string ("Exception caught: ") + std::string (e.what ());
    }
    EXPECT_EQ (this -> my_cartridge -> read (0x8000), 'A');
}

TEST_F (test_cartridge, INES_PROGRAM_CONTENT_ENDING) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "program_content_ending.ines");
    }
    catch (cartridge_exception &e)
    {
        FAIL () << std::string ("Exception caught: ") + std::string (e.what ());
    }
    EXPECT_EQ (this -> my_cartridge -> read (0x8000 + 32767), 'G');
}

TEST_F (test_cartridge, INES_CHARACTER_CONTENT_BEGINNING) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "character_content_beginning.ines");
    }
    catch (cartridge_exception &e)
    {
        FAIL () << std::string ("Exception caught: ") + std::string (e.what ());
    }
    EXPECT_EQ (this -> my_cartridge -> read (0), 'C');
}

TEST_F (test_cartridge, INES_CHARACTER_CONTENT_ENDING) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "character_content_ending.ines");
    }
    catch (cartridge_exception &e)
    {
        FAIL () << std::string ("Exception caught: ") + std::string (e.what ());
    }
    EXPECT_EQ (this -> my_cartridge -> read (8191), 'R');
}

TEST_F (test_cartridge, INES_TRAINER_CONTENT) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "trainer_content.ines");
    }
    catch (cartridge_exception &e)
    {
        FAIL () << std::string ("Exception caught: ") + std::string (e.what ());
    }

    // Checking other cartridge contents aren't shifted
    EXPECT_EQ (this -> my_cartridge -> read (0x8000), 'A');
    EXPECT_EQ (this -> my_cartridge -> read (0x8000 + 32767), 'G');
    EXPECT_EQ (this -> my_cartridge -> read (0), 'T');
    EXPECT_EQ (this -> my_cartridge -> read (8191), 'A');

    EXPECT_EQ (this -> my_cartridge -> read (0x7000), 0x00);
    EXPECT_EQ (this -> my_cartridge -> read (0x7000 + 511), 0x00);
}

TEST_F (test_cartridge, INES_UNSUPPORTED_MAPPER) // NOLINT
{
    try
    {
        this -> my_cartridge = new cartridge(ROM_FILES_PATH "unsupported_mapper.ines");
    }
    catch (cartridge_exception &e)
    {
        EXPECT_EQ (std::string ("iNES: Unsupported mapper"), std::string (e.what ()));
    }
}