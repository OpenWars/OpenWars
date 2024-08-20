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
#ifndef __openwars__game__sound__sound__hpp__
#define __openwars__game__sound__sound__hpp__

#include "../nuclei.hpp"

namespace OpenWars {
	typedef struct {
		u64			samples;
		// (-1.0)...(1.0) samples.
		float		*data;
		float		time;
		float		duration;

		uintptr_t	data_ptr;
	} sound_t;

	ErrorOr<void> init_audio(void);
	ErrorOr<void> deinit_audio(void);
	
	ErrorOr<sound_t *> load_sound_from_file(const char *filepath);
	ErrorOr<void> update_sound(sound_t *s);
	void free_sound(sound_t *s);
};

#endif
