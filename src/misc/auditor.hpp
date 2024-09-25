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
#ifndef __openwars__auditor__hpp__
#define __openwars__auditor__hpp__

#include "../nuclei.hpp"

namespace OpenWars {
	typedef enum {
		EMPTY,
		TEXTURE,
		AUDIO_STREAM,
		FONT,
		MISC,
	} AUDITOR_RESOURCES;

	i8 init_auditor(const char **err);
	void deinit_auditor(void);

	u64 audit(u32 res, const char *add, void *addr, const char **err);
	u64 audit(u32 res, const char *add, const char **err);
	u64 audit(const char **err);
	i8 deaudit(u64 id, const char **err);
};

#endif
