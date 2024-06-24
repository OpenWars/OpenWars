#ifndef __openwars__io__log__cpp__
#define __openwars__io__log__cpp__

#include "log.hpp"
#include <cstdio>
#include <new>

namespace OpenWars {
	int default_log_debug(const char *s) {
		#ifdef OPENWARS_DEBUG
			(void)s;
		#else
			(void)s;
		#endif
	};

	int log_f_debug(const char *s) {
		return default_log_debug(s);
	};

	int log_f_info(const char *s) {
		return default_log_info(s);
	};

	int log_f_error(const char *s) {
		return default_log_error(s);
	};

	int log_debug(const char *format, ...) {
		char *buff = nullptr;

		try {
			buff = new char[16385];
		} catch(std::bad_alloc& e) {
			return -1;
		};

		va_list args;
		va_start(args, format);

		int res = std::vsnprintf(buff, 16385, format, args);

		va_end(args);

		if(res != 0) {
			delete[] buff;
			return res;
		}

		res = log_f_debug(buff);

		delete[] buff;

		return res;
	};
};

#endif
