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
#ifndef __openwars__nuclei__hpp__
#define __openwars__nuclei__hpp__

#include "nuclei.hpp"
#include "io/log.hpp"

namespace OpenWars {
	void __assert_me__(const char *file, int line, const char *comment) {
		if(comment == nullptr) {
			log_error("Assert at %s#%i has failed.\n", file, line);
		} else {
			log_error("Assert at %s#%i has failed: %s.\n", file, line, comment);
		}

		std::exit(1);
	};

	void vfree(void *p) {
		if(p == nullptr)
			return;

		std::free(p);
	};
};

#endif
