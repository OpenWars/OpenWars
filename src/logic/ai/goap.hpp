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
#ifndef __openwars__game__ai__goap__hpp__
#define __openwars__game__ai__goap__hpp__

#include "../../defs.hpp"
#include <vector>
#include <unordered_map>

namespace OpenWars {
	namespace GOAP {
		//typedef std::unordered_map<const char *, uintptr_t> state_t;


		class Planner {
			private:
				actions_t actions;
				plan_t *last_plan = nullptr;

			public:
				void clear(void);

				void add_action(action_t action);
				plan_t get_plan(state_t initial, goal_t goal);
		};
	};
};

#endif
