#ifndef __openwars__task__task__hpp__
#define __openwars__task__task__hpp__

#include "../../defs.hpp"
#include <thread>
#include <queue>
#include <mutex>

namespace OpenWars {
	namespace Tasks {
		typedef void (*task_callback_t)(void);

		typedef struct {
			u32 id;
			bool report;
			task_callback_t callback;
		} task_t;

		class Pawn {
			private:
				const char *err_str = nullptr;
				std::thread *thread_ptr = nullptr;

			public:
				~Pawn(void);

				const char *get_error(void);

				int create(std::queue<task_t> *tasks_queue, std::queue<u32> *finished_tasks, std::mutex *tasks_mutex);
				bool created(void);
				void destruct(void);
		};

		class King {
			private:
				const char *err_str = nullptr;

				Pawn *pawns = nullptr;
				unsigned int number_of_pawns = 0;

				std::queue<task_t> tasks_queue;
				std::queue<u32> finished_tasks;
				std::mutex tasks_mutex;

				u32 inc_task_id = 0x00000000;

			public:
				~King(void);

				const char *get_error(void);
				unsigned int get_cpu_threads(void);

				bool created(void);
				int init_pawns(void);
				void deinit_pawns(void);

				u32 schedule(task_callback_t callback, bool report);
				u32 schedule(task_callback_t callback);
		};
	};
};

#endif
