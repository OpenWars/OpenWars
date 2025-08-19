#pragma once
#include <cstdarg>
#include <string>

namespace OpenWars::IO::Logging {
    void init();
    void log(std::string message, std::va_list args);
    void warn(std::string message, std::va_list args);
    void debug(std::string message, std::va_list args);
    void error(std::string message, std::va_list args, bool fatal);
    void out(int type, const char* message, std::va_list args);
} // namespace OpenWars::IO::Logging