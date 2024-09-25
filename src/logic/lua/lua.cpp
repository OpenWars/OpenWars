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
#ifndef __openwars__game__lua__lua__cpp__
#define __openwars__game__lua__lua__cpp__

#include "lua.hpp"

namespace OpenWars {
	LuaState::LuaState(void) {
		sol_state.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
		
	};

	LuaState::~LuaState(void) {};

	int LuaState::run_script(const char *path, const char **err) {
    (void)err;

		sol::protected_function_result res = sol_state.script_file(path, &sol::script_default_on_error);

		if(res.valid())
			return 0;
		
		return -1;
	};
};

#endif
