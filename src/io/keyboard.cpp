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
#ifndef __openwars__io__keyboard__cpp__
#define __openwars__io__keyboard__cpp__

#include "keyboard.hpp"

namespace Raylib {
	#include <raylib.h>
};

namespace OpenWars {
	bool kbd_is_down(u32 key) {
		int r_key = 0;

		if(key == KBD_KEYS::UP) r_key = Raylib::KEY_UP;
		if(key == KBD_KEYS::LEFT) r_key = Raylib::KEY_LEFT;
		if(key == KBD_KEYS::RIGHT) r_key = Raylib::KEY_RIGHT;
		if(key == KBD_KEYS::DOWN) r_key = Raylib::KEY_DOWN;

		// [TODO] : Handle more keys.

		assert_me(r_key != 0, "(is_kbd_down) 'r_key' shouldn't be NULL");

		return Raylib::IsKeyDown(r_key);
	};

	bool kbd_is_up(u32 key) {
		return (kbd_is_down(key) == false);
	};

	u32 kbd_pop_unicode(void) {
		return (u32)(Raylib::GetCharPressed());
	};
};

#endif
