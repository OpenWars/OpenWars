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
#ifndef __openwars__io__mouse__cpp__
#define __openwars__io__mouse__cpp__

#include "mouse.hpp"

namespace Raylib {
	#include <raylib.h>
};

namespace OpenWars {
	float get_mouse_position_x(void) {
		return (float)Raylib::GetMouseX();
	};

	float get_mouse_position_y(void) {
		return (float)Raylib::GetMouseY();
	};

	float get_mouse_wheel(void) {
		return Raylib::GetMouseWheelMove();
	};

	bool is_mouse_down(void) {
		return Raylib::IsMouseButtonDown(Raylib::MOUSE_BUTTON_LEFT);
	};

	bool is_mouse_alt_down(void) {
		return Raylib::IsMouseButtonDown(Raylib::MOUSE_BUTTON_RIGHT);
	};

	float get_mouse_wheel_x(void) {
		return Raylib::GetMouseWheelMoveV().x;
	};

	float get_mouse_wheel_y(void) {
		return Raylib::GetMouseWheelMoveV().x;
	};
};

#endif
