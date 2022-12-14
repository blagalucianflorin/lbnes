//
// Created by lblaga on 19.03.2022.
//

#ifndef CARTRIDGE_EXCEPTION_HPP
#define CARTRIDGE_EXCEPTION_HPP

#include <exception>
#include <string>
#include <utility>


class cartridge_exception : public std::exception
{
private:
    std::string message;

public:
    inline explicit cartridge_exception (std::string message) { this -> message = std::move (message); }

    inline virtual const char *what () const noexcept { return message.c_str (); } // NOLINT
};

#endif //CARTRIDGE_EXCEPTION_HPP
