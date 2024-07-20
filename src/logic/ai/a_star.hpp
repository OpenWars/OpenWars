#ifndef __openwars__game__ai__a_star__hpp__
#define __openwars__game__ai__a_star__hpp__

#include "../../defs.hpp"
#include <vector>

namespace OpenWars {
	namespace AStar {
		typedef void *state_t;

		typedef state_t (* copy_state_t)(state_t);
		typedef void (* free_state_t)(state_t);

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

		class Pathfinder {
			private:
				actions_t actions;
				copy_state_t copy_state;
				free_state_t free_state;
			
			public:
				void clear(void);

				void add_action(action_t action);
				void set_copy(copy_state_t f);
				void set_free(free_state_t f);

				plan_t get_plan(state_t initial, goal_t goal);
		};
	};
};

#endif
