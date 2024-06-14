#ifndef __openwars__task__task__cpp__
#define __openwars__task__task__cpp__

#include "task.hpp"
#include <new>
#include <chrono>

#include <cstdio>

namespace OpenWars {
	namespace Tasks {
		void pawn_wait(void) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		};

		void pawn_loop(std::queue<task_t> *tasks_queue, std::queue<u32> *finished_tasks, std::mutex *tasks_mutex) {
			task_t curr_task;

			while(true) {
				// If it's blocked, wait 1 ms and check again.
				while(tasks_mutex->try_lock() == false) {
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				};

				if(tasks_queue->empty()) {
					curr_task.id = 0x00000000;
					curr_task.report = false;
					curr_task.callback = pawn_wait;
				} else {
					curr_task = tasks_queue->front();

					// "BRK".
					if(curr_task.id != 0xffffff00)
						tasks_queue->pop();
				}

				tasks_mutex->unlock();

				// "BRK".
				if(curr_task.id == 0xffffff00)
					break;

				curr_task.callback();

				// [TODO] : Mutex-ify "finished_tasks".
				if(curr_task.report)
					finished_tasks->push(curr_task.id);					
			};
		};

		// Class `Pawn`.
		Pawn::~Pawn(void) {
			destruct();
		};

		const char *Pawn::get_error(void) {
			return err_str;
		};

		int Pawn::create(std::queue<task_t> *tasks_queue, std::queue<u32> *finished_tasks, std::mutex *tasks_mutex) {
			if(created()) return -1;

			try {
				thread_ptr = new std::thread(pawn_loop, tasks_queue, finished_tasks, tasks_mutex);
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
			if(thread_ptr != nullptr)
				delete thread_ptr;
		};

		// Class `King`.
		King::~King(void) {
			deinit_pawns();
		};

		const char *King::get_error(void) {
			return err_str;
		};

		// Is the processor lying? Idk, and I don't care.
		unsigned int King::get_cpu_threads(void) {
			return std::thread::hardware_concurrency();
		};

		bool King::created(void) {
			return (number_of_pawns > 0);
		};

		int King::init_pawns(void) {
			if(created()) {
				err_str = "All pawns are alive";
				return -1;
			}

			unsigned int n = get_cpu_threads();
			if(n < 1) n = 1;

			try {
				pawns = new Pawn[n];
			} catch(std::bad_alloc& e) {
				err_str = "Couldn't allocate enough memory for the pawns";
				return -1;
			};

			number_of_pawns = n;

			for(unsigned int i = 0; i < n; i++) {
				if(pawns[i].create(&tasks_queue, &finished_tasks, &tasks_mutex) < 0) {
					err_str = pawns[i].get_error();
					deinit_pawns();

					return -1;
				}
			};

			return 0;
		};

		void King::deinit_pawns(void) {
			if(created() == false) return;

			for(unsigned int i = 0; i < number_of_pawns; i++)
				pawns[i].destruct();
			
			delete[] pawns;
			number_of_pawns = 0;

			while(tasks_queue.empty() == false)
				tasks_queue.pop();

			while(finished_tasks.empty() == false)
				finished_tasks.pop();
			
			inc_task_id = 0x00000000;
		};

		u32 King::schedule(task_callback_t callback, bool report) {
			if(created() == false) return (-1);

			u32 id = report
				? (inc_task_id++)
				: 0x00000000;

			tasks_queue.push(task_t {
				id,
				report,
				callback,
			});

			return id;
		};

		u32 King::schedule(task_callback_t callback) {
			return schedule(callback, false);
		};
	};
};

#endif
