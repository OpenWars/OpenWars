#include "logging.hpp"
#include "../graphics/graphics.hpp"
#include <cstdio>
#include <ctime>

namespace OpenWars::IO::Logging {
    const char *_TEXT_DEBUG_LOG = "\x1b[96m[\x1b[95mDEBUG\x1b[96m]\x1b[0m ",
               *_TEXT_INFO_LOG = "\x1b[93m[\x1b[33mINFO\x1b[93m]\x1b[0m ",
               *_TEXT_ERROR_LOG = "\x1b[97m[\x1b[31mERROR\x1b[97m]\x1b[0m ";
};

void OpenWars::IO::Logging::init() {
    raylib::SetTraceLogCallback(OpenWars::IO::Logging::out);
}

void OpenWars::IO::Logging::out(int type, const char *message,
                                std::va_list args) {
    char timeStr[64] = {0};
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("[%s] ", timeStr);

    switch (type) {
    case raylib::LOG_INFO:
        printf("%s", OpenWars::IO::Logging::_TEXT_INFO_LOG);
        break;
    case raylib::LOG_ERROR:
        printf("%s", OpenWars::IO::Logging::_TEXT_ERROR_LOG);
        break;
    case raylib::LOG_WARNING:
        printf("%s", OpenWars::IO::Logging::_TEXT_INFO_LOG);
        break;
    case raylib::LOG_DEBUG:
        printf("%s", OpenWars::IO::Logging::_TEXT_DEBUG_LOG);
        break;
    default:
        break;
    }

    vprintf(message, args);
    printf("\n");
}
