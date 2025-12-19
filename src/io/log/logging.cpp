#include "logging.hpp"
#include <SDL3/SDL_log.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>

namespace OpenWars::IO::Logging {
    const char *_TEXT_DEBUG_LOG = "\x1b[96m[\x1b[95mDEBUG\x1b[96m]\x1b[0m ",
               *_TEXT_INFO_LOG = "\x1b[93m[\x1b[33mINFO\x1b[93m]\x1b[0m ",
               *_TEXT_ERROR_LOG = "\x1b[97m[\x1b[31mERROR\x1b[97m]\x1b[0m ";
}

void OpenWars::IO::Logging::init(int argc, char* argv[]) {
    SDL_SetLogOutputFunction(
        [](void* userdata,
           int category,
           SDL_LogPriority priority,
           const char* message) {
            char timeStr[64] = {0};
            time_t now = time(NULL);
            struct tm* tm_info = localtime(&now);

            strftime(timeStr, sizeof(timeStr), "%d-%m-%Y %H:%M:%S", tm_info);
            printf("[%s] ", timeStr);

            switch(priority) {
            case SDL_LOG_PRIORITY_INFO:
                printf("%s", OpenWars::IO::Logging::_TEXT_INFO_LOG);
                break;
            case SDL_LOG_PRIORITY_ERROR:
            case SDL_LOG_PRIORITY_CRITICAL:
                printf("%s", OpenWars::IO::Logging::_TEXT_ERROR_LOG);
                break;
            case SDL_LOG_PRIORITY_WARN:
                printf("%s", OpenWars::IO::Logging::_TEXT_INFO_LOG);
                break;
            case SDL_LOG_PRIORITY_TRACE:
            case SDL_LOG_PRIORITY_VERBOSE:
            case SDL_LOG_PRIORITY_DEBUG:
            case SDL_LOG_PRIORITY_COUNT:   // what is this
            case SDL_LOG_PRIORITY_INVALID: // what is this too
                printf("%s", OpenWars::IO::Logging::_TEXT_DEBUG_LOG);
                break;
            default:
                break;
            }

            printf("%s\n", message);
        },
        nullptr
    );

    SDL_LogPriority priority = SDL_LOG_PRIORITY_INFO;

    // todo: clean this?
    for(int i = 1; i < argc; ++i) {
        if(std::strncmp(argv[i], "--log=", 6) == 0) {
            const char* value = argv[i] + 6;

            if(std::strcmp(value, "debug") == 0)
                priority = SDL_LOG_PRIORITY_DEBUG;
            else if(std::strcmp(value, "verbose") == 0)
                priority = SDL_LOG_PRIORITY_VERBOSE;
            else if(std::strcmp(value, "trace") == 0)
                priority = SDL_LOG_PRIORITY_TRACE;
            else if(std::strcmp(value, "info") == 0)
                priority = SDL_LOG_PRIORITY_INFO;
            else if(std::strcmp(value, "warn") == 0)
                priority = SDL_LOG_PRIORITY_WARN;
            else if(std::strcmp(value, "error") == 0)
                priority = SDL_LOG_PRIORITY_ERROR;
            else if(std::strcmp(value, "critical") == 0)
                priority = SDL_LOG_PRIORITY_CRITICAL;
        }
    }

    // hack: avoid sdl internal messages, maybe we can do this on log callback?
    if(priority != SDL_LOG_PRIORITY_INFO) {
        SDL_SetLogPriorities(priority);
    }
}

void OpenWars::IO::Logging::out(
    int priority,
    const char* message,
    std::va_list args
) {
    char buffer[4096];
    vsnprintf(buffer, sizeof(buffer), message, args);
    SDL_LogMessage(
        SDL_LOG_CATEGORY_APPLICATION,
        (SDL_LogPriority)priority,
        "%s",
        buffer
    );
}