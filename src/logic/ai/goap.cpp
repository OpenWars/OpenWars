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
#ifndef __openwars__game__ai__goap__cpp__
#define __openwars__game__ai__goap__cpp__

#include "goap.hpp"
#include <cstdio>
#include <new>

namespace OpenWars {
	namespace GOAP {
		void Planner::clear(void) {
			actions.clear();
			
			if(last_plan != nullptr)
				delete last_plan;
		};

		void Planner::add_action(action_t action) {
			actions.push_back(action);
		};

		u32 planner_get_heuristic(state_t state, goal_t goal) {
			u32 h = 0;

			for(u32 i = 0; i < goal.size(); i++) {
				if(goal[i](state))
					h++;
			};

			return h;
		};

		typedef struct {
			u32			cost;
			state_t		state;
			actions_t	actions;
		} planner_node_t;

		std::size_t planner_get_node_hash(const planner_node_t &node) {
			std::size_t h = 0xabababababababab;

			for(u32 i = 0; i < node.actions.size(); i++) {
				h <<= 1;
				h ^= node.actions[i].id;
			};

			h ^=	(node.actions.size() << 3) +
					(node.actions.size() << 2) +
					node.actions.size();

			return h;
		};

		struct planner_node_hash_t {
			std::size_t operator()(const planner_node_t &node) const {
				return planner_get_node_hash(node);
			};
		};

		struct planner_node_equal_t {
			bool operator()(const planner_node_t &left, const planner_node_t &right) const {
				return planner_get_node_hash(left) == planner_get_node_hash(right);
			};
		};

		struct list_node_t {
			void			*prev = nullptr;
			void			*next = nullptr;
			planner_node_t	node;
		};

		class PlannerNodeList {
			private:
				list_node_t *front_node = nullptr;
				list_node_t *back_node = nullptr;

			public:
				bool empty(void) {
					return (front_node == nullptr);
				};

				list_node_t *back(void) {
					return back_node;
				};

				list_node_t *front(void) {
					return front_node;
				};

				i8 insert(list_node_t *to, planner_node_t node) {
					list_node_t *c_node;

					try {
						c_node = new list_node_t;
					} catch(std::bad_alloc &e) {
						return -1;
					};

					c_node->node = node;

					if(to != nullptr) {
						c_node->next = to->next;
						to->next = c_node;
					} else {
						front_node = c_node;
					}

					if(c_node->next != nullptr) {
						((list_node_t *)c_node->next)->prev = c_node;
					} else {
						back_node = c_node;
					}

					return 0;
				};

				void remove(list_node_t *node) {
					if(node->prev != nullptr) {
						((list_node_t *)node->prev)->next = node->next;
						front_node = (list_node_t *)node->prev;
					} else {
						front_node = (list_node_t *)node->next;
					}

					if(node->next != nullptr) {
						((list_node_t *)node->next)->prev = node->prev;
						back_node = (list_node_t *)node->next;
					} else {
						back_node = (list_node_t *)node->prev;
					}

					delete node;
				};

				int push_back(planner_node_t node) {
					return insert(back_node, node);
				};

				list_node_t *next(list_node_t *node) {
					if(node == nullptr)
						return nullptr;

					return (list_node_t *)node->next;
				};
		};

		plan_t Planner::get_plan(state_t initial, goal_t goal) {
			PlannerNodeList open_list;
			std::unordered_map<planner_node_t, u8, planner_node_hash_t, planner_node_equal_t> closed_list;

			actions_t empty_actions;

			open_list.push_back({
				0,
				initial,
				empty_actions,
			});

			while(open_list.empty() == false) {
				list_node_t *curr_node = open_list.front();

				for(list_node_t *n = curr_node; n != nullptr; n = open_list.next(n)) {
					if(n->node.cost < curr_node->node.cost)
						curr_node = n;
				};

				closed_list[curr_node->node] = 0xaa;

				if(planner_get_heuristic(curr_node->node.state, goal) == goal.size()) {
					return {
						true,
						curr_node->node.cost,
						curr_node->node.actions,
					};
				}

				action_t action;
				for(u32 i = 0; i < actions.size(); i++) {
					action = actions[i];

					if(action.condition(curr_node->node.state) == false)
						continue;
					if(closed_list[curr_node->node] == 0xaa)
						continue;

					planner_node_t new_node = {
						action.cost(curr_node->node.state),
						action.react(curr_node->node.state),
						curr_node->node.actions,
					};

					new_node.actions.push_back(action);

					// [TODO]

					open_list.push_back(new_node);
				};

				open_list.remove(curr_node);
			};

			return {
				false,
				0,
				empty_actions,
			};
		};
	};
};

#endif
