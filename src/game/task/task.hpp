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

		typedef std::queue<task_t> tasks_queue_t;

		class Pawn {
			private:
				const char *err_str = nullptr;
				std::thread *thread_ptr = nullptr;

			public:
				~Pawn(void);

				const char *get_error(void);

				int create(tasks_queue_t *tasks_queue, std::mutex *tasks_mutex);
				bool created(void);
				void destruct(void);
		};

		class King {
			private:
				const char *err_str = nullptr;

				bool created = false;
				Pawn *pawns = nullptr;
				unsigned int number_of_task_handlers = 0;

				tasks_queue_t tasks_queue;
				std::mutex tasks_mutex;

			public:
				~King(void);

				const char *get_error(void);
				unsigned int get_cpu_threads(void);

				int init_pawns(void);
				void deinit_pawns(void);

				u32 schedule(void);
		};
	};
};

#endif
