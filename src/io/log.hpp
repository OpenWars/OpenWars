#ifndef __openwars__io__log__hpp__
#define __openwars__io__log__hpp__

#include <cstdarg>

namespace OpenWars {
	typedef int (*log_callback_t)(void);

	// \x1b[93m[\x1b[91mOpen[31mWars[93m]\x1b[0m
	// \x1b[96m[\x1b[95mDEBUG\x1b[96m]\x1b[0m
	// \x1b[91m[\x1b[41mINFO\x1b[91m]\x1b[0m
	// \x1b[97m[\x1b[41mERROR\x1b[97m]\x1b[0m

	void log_debug(const char *format, ...);
	void log_info(const char *format, ...);
	void log_error(const char *format, ...);
};

#endif
