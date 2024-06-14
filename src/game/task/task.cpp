#ifndef __openwars__task__task__cpp__
#define __openwars__task__task__cpp__

#include "task.hpp"
#include <new>
#include <chrono>

namespace OpenWars {
	namespace Tasks {
		void pawn_loop(tasks_queue_t *tasks_queue_ptr, std::mutex *tasks_mutex) {
			task_t curr_task;

			while(true) {
				// While it's blocked (if it's blocked), wait 1ms and check again.
				while(tasks_mutex->try_lock() == false) {
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				};

				if(tasks_queue_ptr->empty()) {
					curr_task.id = 0x00000000;
					curr_task.report = false;
					curr_task.callback = [](void) {};
				} else {
					curr_task = tasks_queue_ptr->front();
					tasks_queue_ptr->pop();
				}

				tasks_mutex->unlock();
			};
		};

		// Class `Pawn`.
		Pawn::~Pawn(void) {
			// [TODO]
		};

		const char *Pawn::get_error(void) {
			return err_str;
		};

		int Pawn::create(tasks_queue_t *tasks_queue, std::mutex *tasks_mutex) {
			if(created()) return -1;

			try {
				thread_ptr = new std::thread(pawn_loop, tasks_queue, tasks_mutex);
			} catch(std::bad_alloc& e) {
				err_str = "Couldn't allocate a Thread";
				return -1;
			};

			thread_ptr->detach();

			return 0;
		};

		bool Pawn::created(void) {
			return (thread_ptr != nullptr);
		};

		void Pawn::destruct(void) {
			// [TODO]
		};

		// Class `King`.
		King::~King(void) {
			// [TODO]
		};

		const char *King::get_error(void) {
			return err_str;
		};

		// Is the processor lying? Idk, and I don't care.
		unsigned int King::get_cpu_threads(void) {
			return std::thread::hardware_concurrency();
		};

		int King::init_pawns(void) {
			// [TODO]
			return -7050;
		};

		void King::deinit_pawns(void) {
			// [TODO]
		};

		u32 King::schedule(void) {
			// [TODO]
			return -7050;
		};
	};
	
	/*
	TasksList::task_callback_t TasksList::pop_task(void) {
		// [TODO]
	};

	TaskHandler::TaskHandler(void) {};

	TaskHandler::~TaskHandler(void) {
		destruct();
	};

	const char *TaskHandler::get_error(void) {
		return err_str;
	};

	int TaskHandler::create(TasksList *tasks_ptr) {
	};

	bool TaskHandler::created(void) {
		return (t_ptr != nullptr);
	};
	
	void TaskHandler::destruct(void) {
		// [TODO]
		delete t_ptr;
	};

	TaskScheduler::TaskScheduler(void) {
		// [TODO]
	};

	TaskScheduler::~TaskScheduler(void) {
		if(task_handlers == nullptr) return;
		destruct_handlers();
	};

	const char *TaskScheduler::get_error(void) {
		return err_str;
	};

	int TaskScheduler::create_handlers(void) {
		if(created == true) return -1;

		unsigned int tn = get_cpu_threads();
		if(tn < 2) tn = 2;

		tn--;

		try {
			task_handlers = new TaskHandler[tn - 1];
		} catch(std::bad_alloc& e) {
			err_str = "Couldn't allocate enough memory for task handlers";
			return -1;
		};

		for(unsigned int i = 0; i < tn; i++) {
			if(task_handlers[i].create(&tasks_queue) < 0) {
				err_str = task_handlers[i].get_error();
				destruct_handlers();

				return -1;
			}
		};

		number_of_task_handlers = tn;
	};

	void TaskScheduler::destruct_handlers(void) {
		for(unsigned int i = 0; i < number_of_task_handlers; i++)
			task_handlers[i].destruct();
	};

	int TaskScheduler::schedule(void) {
		// [TODO]

		return -1;
	};
	*/
};

#endif
