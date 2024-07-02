#ifndef __openwars__task__task__cpp__
#define __openwars__task__task__cpp__

#include "task.hpp"
#include <new>
#include <chrono>
#include <thread>

namespace OpenWars {
	namespace Tasks {
		int pawn_wait(void) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			return 0;
		};

		void pawn_loop(pawn_ctx_t *pawn_ctx) {
			task_t curr_task;
			int task_status = 0;

			while(true) {
				if(pawn_ctx->should_deinit)
					break;

				// If it's blocked, wait 1 ms and check again.
				while(pawn_ctx->mtx_tasks.try_lock() == false)
					(void)pawn_wait();

				if(pawn_ctx->tasks.empty()) {
					curr_task.success = nullptr;
					curr_task.failure = nullptr;
					curr_task.callback = pawn_wait;
				} else {
					curr_task = pawn_ctx->tasks.front();
					pawn_ctx->tasks.pop();
				}

				pawn_ctx->mtx_tasks.unlock();

				task_status = curr_task.callback();

				if((task_status == 0) & (curr_task.success != nullptr)) {
					while(pawn_ctx->mtx_fin.try_lock() == false)
						(void)pawn_wait();

					pawn_ctx->fin.push(curr_task.success);
					pawn_ctx->mtx_fin.unlock();
				}

				if((task_status < 0) & (curr_task.failure != nullptr)) {
					while(pawn_ctx->mtx_fin.try_lock() == false)
						(void)pawn_wait();

					pawn_ctx->fin.push(curr_task.failure);
					pawn_ctx->mtx_fin.unlock();
				}
			};

			while(pawn_ctx->mtx_fin.try_lock() == false)
				(void)pawn_wait();

			pawn_ctx->deinit_count++;

			pawn_ctx->mtx_fin.unlock();			
		};

		// Class `Pawn`.
		Pawn::~Pawn(void) {
			deinit();
		};

		const char *Pawn::get_error(void) {
			return err_str;
		};

		int Pawn::init(pawn_ctx_t *pawn_ctx) {
			if(initialized()) return -1;

			try {
				thread_ptr = new std::thread(pawn_loop, pawn_ctx);
			} catch(std::bad_alloc& e) {
				err_str = "Couldn't allocate a Thread";
				return -1;
			};

			thread_ptr->detach();

			return 0;
		};

		bool Pawn::initialized(void) {
			return (thread_ptr != nullptr);
		};

		void Pawn::deinit(void) {
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

		bool King::initialized(void) {
			return (number_of_pawns > 0);
		};

		int King::init_pawns(void) {
			if(initialized()) {
				err_str = "All pawns are alive";
				return -1;
			}

			pawn_ctx.should_deinit = false;

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
				if(pawns[i].init(&pawn_ctx) < 0) {
					err_str = pawns[i].get_error();
					deinit_pawns();

					return -1;
				}
			};

			return 0;
		};

		void King::deinit_pawns(void) {
			if(initialized() == false) return;

			pawn_ctx.should_deinit = true;

			while(true) {
				wait();

				while(pawn_ctx.mtx_fin.try_lock() == false)
					wait();

				if(pawn_ctx.deinit_count == number_of_pawns) break;
				pawn_ctx.mtx_fin.unlock();
			};
			
			delete[] pawns;

			pawn_ctx.should_deinit = false;
			pawn_ctx.deinit_count = 0;
			number_of_pawns = 0;

			while(pawn_ctx.tasks.empty() == false)
				pawn_ctx.tasks.pop();

			while(pawn_ctx.fin.empty() == false)
				pawn_ctx.fin.pop();
		};

		int King::push(const char *success, const char *failure, task_callback_t callback) {
			if(initialized() == false) return -1;

			pawn_ctx.tasks.push(task_t {
				success,
				failure,
				callback,
			});

			return 0;
		};

		int King::push(task_callback_t callback) {
			return push(nullptr, nullptr, callback);
		};

		fin_queue_t * King::lock_fin(void) {
			if(fin_locked) return (&pawn_ctx.fin);

			while(pawn_ctx.mtx_fin.try_lock() == false)
				pawn_wait();

			fin_locked = true;
			
			return (&pawn_ctx.fin);
		};

		void King::unlock_fin(void) {
			if(fin_locked)
				pawn_ctx.mtx_fin.unlock();
		};

		void King::wait(void) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		};
	};
};

#endif
