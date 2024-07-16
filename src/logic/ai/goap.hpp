#ifndef __openwars__game__ai__goap__hpp__
#define __openwars__game__ai__goap__hpp__

#include "../../defs.hpp"
#include <vector>
#include <unordered_map>

namespace OpenWars {
	namespace GOAP {
		typedef std::unordered_map<const char *, u64> state_t;

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
			u32			cost;
			actions_t	actions;
		} plan_t;

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
