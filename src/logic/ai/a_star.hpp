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
#ifndef __openwars__game__ai__a_star__hpp__
#define __openwars__game__ai__a_star__hpp__

#include "../../defs.hpp"
#include <vector>

namespace OpenWars {
	template<typename state_t>
	class AStar {
		public:
			typedef u32 (*cost_t)(state_t state);
			typedef bool (*condition_t)(state_t state);
			typedef state_t (*reaction_t)(state_t state);

			typedef struct {
				u64			id;
				condition_t	condition;
				reaction_t	react;
				cost_t		cost;
			} action_t;

			typedef struct std::vector<condition_t> goal_t;
			typedef std::vector<action_t> actions_t;

			typedef struct {
				bool		found;
				u32			cost;
				actions_t	actions;
			} plan_t;

		private:
			actions_t actions;
			
		public:
			void clear(void);
			void add_action(action_t action);
			plan_t get_plan(state_t initial, goal_t goal);
	};
};

#endif
