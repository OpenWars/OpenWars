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
#ifndef __openwars__task__task__hpp__
#define __openwars__task__task__hpp__

#include "../../nuclei.hpp"
#include <thread>
#include <queue>
#include <mutex>

namespace OpenWars {
	namespace Tasks {
		typedef int (*task_callback_t)(void);

		typedef struct {
			const char		*success;
			const char		*failure;
			task_callback_t	callback;
		} task_t;

		typedef std::queue<const char *> fin_queue_t;
		typedef std::queue<task_t> task_queue_t;

		typedef struct _pawn_ctx_t {
			std::mutex mtx_tasks;
			std::mutex mtx_fin;

			task_queue_t tasks;

			fin_queue_t fin;
			u8 fin_i = 0;

			bool should_deinit = false;
			unsigned int deinit_count = 0;
		} pawn_ctx_t;

		class Pawn {
			private:
				std::thread *thread_ptr = nullptr;

			public:
				~Pawn(void);

				const char *get_error(void);

				i8 init(pawn_ctx_t *pawn_ctx, const char **err);
				bool initialized(void);
				void deinit(void);
		};

		class King {
			private:
				const char **err_str = nullptr;

				Pawn *pawns = nullptr;
				unsigned int number_of_pawns = 0;

				pawn_ctx_t pawn_ctx;
				bool fin_locked = false;

			public:
				~King(void);

				const char *get_error(void);
				unsigned int get_cpu_threads(void);

				bool initialized(void);
				i8 init_pawns(unsigned int min_pawns, const char **err);
				void deinit_pawns(void);

				i8 push(const char *success, const char *failure, task_callback_t callback, const char **err);
				i8 push(task_callback_t callback, const char **err);

				fin_queue_t * lock_fin(void);
				void unlock_fin(void);
				void wait(void);
		};
	};
};

#endif
