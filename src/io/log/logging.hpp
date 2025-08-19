#pragma once

#include <cstdarg>

namespace raylib {
#include "raylib.h"
}

namespace OpenWars::IO::Logging {
    void init();

    template <typename... Args> void log(const char* fmt, Args... args) {
        TraceLog(raylib::LOG_INFO, fmt, args...);
    }

    template <typename... Args> void warn(const char* fmt, Args... args) {
        TraceLog(raylib::LOG_WARNING, fmt, args...);
    }

    template <typename... Args> void debug(const char* fmt, Args... args) {
        TraceLog(raylib::LOG_DEBUG, fmt, args...);
    }

    template <typename... Args>
    void error(bool fatal, const char* fmt, Args... args) {
        TraceLog(fatal ? raylib::LOG_FATAL : raylib::LOG_ERROR, fmt, args...);
    }

    void out(int type, const char* message, std::va_list args);
} // namespace OpenWars::IO::Logging