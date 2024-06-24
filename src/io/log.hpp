#ifndef __openwars__io__log__hpp__
#define __openwars__io__log__hpp__

#include <cstdarg>

namespace OpenWars {
	int log_debug(const char *format, ...);
	int log_info(const char *format, ...);
	int log_error(const char *format, ...);
};

#endif
