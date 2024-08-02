/**
 *
   ___                __        __
  / _ \ _ __   ___ _ _\ \      / /_ _ _ __ ___
 | | | | '_ \ / _ \ '_ \ \ /\ / / _` | '__/ __|
 | |_| | |_) |  __/ | | \ V  V / (_| | |  \__ \
  \___/| .__/ \___|_| |_|\_/\_/ \__,_|_|  |___/
       |_|

Copyright (C) 2024 OpenWars Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef __openwars__io__log__cpp__
#define __openwars__io__log__cpp__

#include "log.hpp"
#include <cstdarg>
#include <cstdio>

namespace OpenWars {
	const char	*_TEXT_DEBUG_LOG = "\x1b[96m[\x1b[95mDEBUG\x1b[96m]\x1b[0m ",
				*_TEXT_INFO_LOG = "\x1b[93m[\x1b[33mINFO\x1b[93m]\x1b[0m ",
				*_TEXT_ERROR_LOG = "\x1b[97m[\x1b[31mERROR\x1b[97m]\x1b[0m ";
	
	int log_something(std::FILE *file, const char *pre, const char *format, std::va_list args) {
		(void)std::fprintf(file, "\x1b[93m[\x1b[91mOpen\x1b[31mWars\x1b[93m]\x1b[0m");
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
