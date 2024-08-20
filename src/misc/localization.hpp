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
#ifndef __openwars__misc__localization__hpp__
#define __openwars__misc__localization__hpp__

#include "../nuclei.hpp"
#include "../io/files.hpp"
#include <unordered_map>

namespace OpenWars {
	namespace L10N {
		typedef std::unordered_map<const char *, const char *> info_t;

		i8 init_localization(const char *err);
		void deinit_localization(void);

		i8 load_language(const char *s, const char *err);
		char *get_text(u32 id, info_t &info);
		void free_text(char *text);
	};
};

#endif
