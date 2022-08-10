//
// Created by lblaga on 20.03.2022.
//

#ifndef NEMULATOR_PPU_EXCEPTION_H
#define NEMULATOR_PPU_EXCEPTION_H

#include <exception>
#include <string>
#include <utility>

class ppu_exception : public std::exception
{
private:
    std::string message;

public:
    inline explicit ppu_exception (std::string message) { this -> message = std::move (message); }

    inline virtual const char *what () const noexcept { return message.c_str (); } // NOLINT
};

#endif //NEMULATOR_PPU_EXCEPTION_H
