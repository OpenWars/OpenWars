#ifndef __openwars__io__log__cpp__
#define __openwars__io__log__cpp__

#include "log.hpp"
#include <cstdio>

namespace OpenWars {
	log_callback_t log_f = [](const char *text) -> void {
		return;
	};
};

#endif
