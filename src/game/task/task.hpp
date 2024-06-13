#ifndef __openwars__task__task__hpp__
#define __openwars__task__task__hpp__

#include "../../defs.hpp"
#include <thread>
#include <queue>

namespace OpenWars {
	typedef void (*task_callback_t)(void);
	typedef std::queue<task_callback_t> tasks_queue_t;

	class TaskHandler {
		private:
			const char *err_str = nullptr;

			std::thread *thread_ptr = nullptr;
			tasks_queue_t *tasks_queue = nullptr;

		public:
			const char *get_error(void);

			int create(tasks_queue_t *tasks_ptr);
			bool created(void);
			void destruct(void);
	};

	class TaskScheduler {
		private:
			const char *err_str = nullptr;

			bool created = false;
			TaskHandler *task_handlers = nullptr;
			unsigned int number_of_task_handlers = 0;

			tasks_queue_t tasks_queue;

		public:
			TaskScheduler(void);
			~TaskScheduler(void);

			const char *get_error(void);
			unsigned int get_cpu_threads(void);

			int create_handlers(void);
			void destruct_handlers(void);
			int schedule(void);
	};
};

#endif
