#pragma once
#include <cstdarg>

namespace OpenWars::IO::Logging {
    void init();
    void out(int type, const char* message, std::va_list args);
} // namespace OpenWars::IO::Logging