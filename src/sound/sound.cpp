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
#ifndef __openwars__game__sound__sound__cpp__
#define __openwars__game__sound__sound__cpp__

#include "sound.hpp"
#include <new>

namespace Raylib {
	#include <raylib.h>
};

namespace OpenWars {
	ErrorOr<void> init_audio(void) {
		// For now, it does nothing (Raylib inits audio at InitWindow).
		return Error { nullptr };
	};

	ErrorOr<void> deinit_audio(void) {
		// Sample as `init_audio`.
		return Error { nullptr };
	};

	typedef struct _c_isptr_t {
		Raylib::Music	r_music;
	} _isptr_t;

	ErrorOr<sound_t *> load_sound_from_file(const char *filepath) {
		_isptr_t *ip;
		sound_t *s;

		try {
			ip = new _isptr_t;
			s = new sound_t;
		} catch(std::bad_alloc &e) {
			return Error {
				"Couldn't allocate enough memory for a sound stream"
			};
		};

		// [TODO]
		ip->r_music = Raylib::LoadMusicStream(filepath);
		*s = {
			0,
			nullptr,
			-1.0,
			-1.0,
			(uintptr_t)ip,
		};

		return s;
	};
};

#endif
