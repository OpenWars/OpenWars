#ifndef __openwars__game__lua__lua__cpp__
#define __openwars__game__lua__lua__cpp__

#include "lua.hpp"

namespace OpenWars {
	LuaState::LuaState(void) {
		sol_state.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
		
	};

	LuaState::~LuaState(void) {};

	const char *LuaState::get_error(void) {
		return err_str;
	};

	int LuaState::run_script(const char *path) {
		sol::protected_function_result res = sol_state.script_file(path, &sol::script_default_on_error);

		if(res.valid())
			return 0;
		
		return -1;
	};
};

#endif
