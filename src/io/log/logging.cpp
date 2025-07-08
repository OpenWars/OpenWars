#include "logging.hpp"
#include <cstdio>
#include <ctime>

namespace raylib {
    #include "raylib.h"
}

void OpenWars::IO::Logging::init(){
    raylib::SetTraceLogCallback(OpenWars::IO::Logging::out);
}

void OpenWars::IO::Logging::out(int type, const char *message, va_list args){
    char timeStr[64] = { 0 };
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("[%s] ", timeStr);

    switch (type)
    {
        case raylib::LOG_INFO: printf("%s", OpenWars::IO::Logging::_TEXT_INFO_LOG); break;
        case raylib::LOG_ERROR: printf("%s", OpenWars::IO::Logging::_TEXT_ERROR_LOG); break;
        case raylib::LOG_WARNING: printf("%s", OpenWars::IO::Logging::_TEXT_INFO_LOG); break;
        case raylib::LOG_DEBUG: printf("%s", OpenWars::IO::Logging::_TEXT_DEBUG_LOG); break;
        default: break;
    }

    vprintf(message, args);
    printf("\n");
}