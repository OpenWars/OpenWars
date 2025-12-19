#pragma once

#include <cstdarg>
#include <SDL3/SDL.h>

namespace OpenWars::IO::Logging {
    enum LogLevel {
        LOG_INFO = SDL_LOG_PRIORITY_INFO,
        LOG_WARNING = SDL_LOG_PRIORITY_WARN,
        LOG_ERROR = SDL_LOG_PRIORITY_ERROR,
        LOG_DEBUG = SDL_LOG_PRIORITY_DEBUG,
        LOG_FATAL = SDL_LOG_PRIORITY_CRITICAL
    };

    void init(int argc, char* argv[]);

    template <typename... Args> void log(const char* fmt, Args... args) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, fmt, args...);
    }

    template <typename... Args> void warn(const char* fmt, Args... args) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, fmt, args...);
    }

    template <typename... Args> void debug(const char* fmt, Args... args) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, fmt, args...);
    }

    template <typename... Args>
    void error(bool fatal, const char* fmt, Args... args) {
        if(fatal) {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, fmt, args...);
        } else {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, fmt, args...);
        }
    }

    void out(int priority, const char* message, std::va_list args);
} // namespace OpenWars::IO::Logging