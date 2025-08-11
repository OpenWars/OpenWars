#pragma once
#include <cstdarg>

namespace OpenWars {
    namespace IO {
        namespace Logging {
            void init();
            void out(int type, const char *message, std::va_list args);
        } // namespace Logging
    } // namespace IO
} // namespace OpenWars