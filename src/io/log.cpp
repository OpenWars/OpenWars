#ifndef __openwars__io__log__cpp__
#define __openwars__io__log__cpp__

#include "log.hpp"
#include <cstdio>

namespace OpenWars {
	void log_f_debug(const char *s) {
		#ifdef OPENWARS_DEBUG
			(void)s;
		#else
			(void)s;
		#endif
	};

	void log_f_info(const char *s) {
		std::printf(s);
	};

	void log_f_error(const char *s) {
		std::fprintf(stderr, s);
	};
};

#endif
