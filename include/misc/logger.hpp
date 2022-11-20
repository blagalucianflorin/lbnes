//
// Created by blaga on 20-Nov-22.
// A simple logging utility
//


#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

#if defined (unix) and not defined (__APPLE__)
#define LOGGER_BASE(level, message, output_std) logger::log (level, message, __FILE__, __LINE__, output_std)
#else
#define LOGGER_BASE(level, message, output_std) logger::log (level, message, __FILE_NAME__, __LINE__, output_std)
#endif


#define LOGGER_INFO(message) LOGGER_BASE (logger::INFO, message, false)
#define LOGGER_DEBUG(message) LOGGER_BASE (logger::DBG, message, false)
#define LOGGER_WARNING(message) LOGGER_BASE (logger::WARNING, message, false)
#define LOGGER_ERROR(message) LOGGER_BASE (logger::ERR, message, false)
#define LOGGER_CRITICAL(message) LOGGER_BASE (logger::CRITICAL, message, false)

#define LOGGER_INFO_STD(message) LOGGER_BASE (logger::INFO, message, true)
#define LOGGER_DEBUG_STD(message) LOGGER_BASE (logger::DBG, message, true)
#define LOGGER_WARNING_STD(message) LOGGER_BASE (logger::WARNING, message, true)
#define LOGGER_ERROR_STD(message) LOGGER_BASE (logger::ERR, message, true)
#define LOGGER_CRITICAL_STD(message) LOGGER_BASE (logger::CRITICAL, message, true)

#if _WIN32
#include <windows.h>
inline int cross_platform_mkdir (const std::string& folder_name)\
{
    return (CreateDirectoryA (folder_name.c_str (), nullptr));
}
#else
inline int cross_platform_mkdir (const std::string& folder_name)
{
    return (system ((std::string ("mkdir -p ") + folder_name).c_str ()));
}
#endif


class logger
{
public:
    enum level { INFO, DBG, WARNING, ERR, CRITICAL };

private:
    logger () = default;

    ~logger () { logger::output_file.close (); }

    static std::ofstream output_file;

    inline static std::string get_current_time_date ()
    {
        std::time_t end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string result;

        result += std::ctime(&end_time);
        result.erase (std::remove (result.begin (), result.end (), '\n'), result.end ());

        return result;
    }

public:
    inline static void init (std::string output_file_path)
    {
        cross_platform_mkdir ("logs");

        output_file_path += std::string (" ") + get_current_time_date () + ".log";
        std::replace (output_file_path.begin (), output_file_path.end (), ':', '-');

        logger::output_file.open ("logs/" + output_file_path);
    }

    static void log (level log_level, const std::string& message, const char *file_path, long long line, bool output_to_std)
    {
        std::string file = file_path;

#if defined (unix) and not defined (__APPLE__)
        file = file.substr(file.find_last_of("/\\") + 1);
#endif

        std::string log = get_current_time_date () + " " + file + ":" + std::to_string(line) + ":";

        switch (log_level)
        {
            case INFO:     log += " [INFO] ";     break;
            case DBG:      log += " [DEBUG] ";    break;
            case WARNING:  log += " [WARNING] ";  break;
            case ERR:      log += " [ERROR] ";    break;
            case CRITICAL: log += " [CRITICAL] "; break;
        }

        log += message;

        logger::output_file << log << std::endl;
        if (output_to_std)
            std::cout << log << std::endl;
    }
};

inline std::ofstream logger::output_file;

#endif //LOGGER_HPP

