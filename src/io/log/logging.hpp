#pragma once

namespace OpenWars {
    namespace IO {
        namespace Logging {
            const static char	*_TEXT_DEBUG_LOG = "\x1b[96m[\x1b[95mDEBUG\x1b[96m]\x1b[0m ",
			                    *_TEXT_INFO_LOG = "\x1b[93m[\x1b[33mINFO\x1b[93m]\x1b[0m ",
			                    *_TEXT_ERROR_LOG = "\x1b[97m[\x1b[31mERROR\x1b[97m]\x1b[0m ";

            void init();
            //void out(int type, const char *message, va_list args);
        }
    }
}