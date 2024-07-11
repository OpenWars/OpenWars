#ifndef __openwars__io__log__cpp__
#define __openwars__io__log__cpp__

#include "log.hpp"
#include <cstdarg>
#include <cstdio>

namespace OpenWars {
	const char	*_TEXT_LOG = "\x1b[93m[\x1b[91mOpen\x1b[31mWars\x1b[93m]\x1b[0m",
				*_TEXT_DEBUG_LOG = "\x1b[96m[\x1b[95mDEBUG\x1b[96m]\x1b[0m ",
				*_TEXT_INFO_LOG = "\x1b[93m[\x1b[33mINFO\x1b[93m]\x1b[0m ",
				*_TEXT_ERROR_LOG = "\x1b[97m[\x1b[31mERROR\x1b[97m]\x1b[0m ";
	
	int log_something(std::FILE *file, const char *pre, const char *format, std::va_list args) {
		(void)std::fprintf(file, _TEXT_LOG);
		(void)std::fprintf(file, pre);
		int res = std::vfprintf(file, format, args);

		return res;
	};

	int log_debug(const char *format, ...) {
		va_list args;
		va_start(args, format);

		int res = log_something(stdout, _TEXT_DEBUG_LOG, format, args);

		va_end(args);

		return res;
	};

	int log_info(const char *format, ...) {
		va_list args;
		va_start(args, format);

		int res = log_something(stdout, _TEXT_INFO_LOG, format, args);

		va_end(args);

		return res;
	};

	int log_error(const char *format, ...) {
		va_list args;

		va_start(args, format);
		(void)log_something(stdout, _TEXT_ERROR_LOG, format, args);
		va_end(args);

		va_start(args, format);
		int res = log_something(stderr, _TEXT_ERROR_LOG, format, args);
		va_end(args);

		return res;
	};
};

#endif
