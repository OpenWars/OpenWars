#ifndef __openwars__game__lua__lua__hpp__
#define __openwars__game__lua__lua__hpp__

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace OpenWars {
	class LuaState {
		private:
			const char *err_str = nullptr;
			sol::state sol_state;

		public:
			LuaState(void);
			~LuaState(void);

			const char *get_error(void);
			int load_script(const char *path);
			int unload_script(void);
			int run_script(void);
	};
};

#endif
