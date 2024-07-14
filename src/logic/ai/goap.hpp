#ifndef __openwars__game__ai__goap__hpp__
#define __openwars__game__ai__goap__hpp__

#include "../../defs.hpp"

namespace OpenWars {
	namespace GOAP {
		typedef uintptr_t state_t;

		typedef u32 (*cost_t)(state_t state);
		typedef bool (*condition_t)(state_t state);
		typedef state_t (*reaction_t)(state_t state);

		typedef struct {
			condition_t	condition;
			reaction_t	react;
			cost_t		cost;
		} action_t;

		typedef struct {
			u32			cost;
			u32			length;
			action_t	*actions;
		} plan_t;

		class Planner {
			public:
				void init(void);
				void deinit(void);

				void add_action(action_t action);
				plan_t *get_plan(state_t initial, state_t desired);
		};
	};
};

#endif
