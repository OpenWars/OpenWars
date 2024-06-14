#ifndef __openwars__task__task__cpp__
#define __openwars__task__task__cpp__

#include "task.hpp"
#include <new>
#include <chrono>
#include <thread>

namespace OpenWars {
	namespace Tasks {
		void pawn_wait(void) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		};

		void pawn_loop(pawn_ctx_t *pawn_ctx) {
			task_t curr_task;

			while(true) {
				if(pawn_ctx->should_deinit)
					break;

				// If it's blocked, wait 1 ms and check again.
				while(pawn_ctx->mtx_tasks.try_lock() == false) {
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				};

				if(pawn_ctx->tasks.empty()) {
					curr_task.id = 0x00000000;
					curr_task.report = false;
					curr_task.callback = pawn_wait;
				} else {
					curr_task = pawn_ctx->tasks.front();
					pawn_ctx->tasks.pop();
				}

				pawn_ctx->mtx_tasks.unlock();

				curr_task.callback();

				// [TODO] : Mutex-ify "finished_tasks".
				if(curr_task.report) {
				// If it's blocked, wait 1 ms and check again.
					while(pawn_ctx->mtx_fin.try_lock() == false) {
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					};

					pawn_ctx->fin[pawn_ctx->fin_i] = curr_task.id;
					pawn_ctx->fin_i++;
					pawn_ctx->fin[pawn_ctx->fin_i] = 0x00000000;

					pawn_ctx->mtx_fin.unlock();			
				}
			};

			while(pawn_ctx->mtx_fin.try_lock() == false) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			};

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
				std::this_thread::sleep_for(std::chrono::milliseconds(2));

				while(pawn_ctx.mtx_fin.try_lock() == false) {
					std::this_thread::sleep_for(std::chrono::milliseconds(2));
				};

				if(pawn_ctx.deinit_count == number_of_pawns) break;
				pawn_ctx.mtx_fin.unlock();
			};
			
			delete[] pawns;

			pawn_ctx.should_deinit = false;
			pawn_ctx.deinit_count = 0;
			number_of_pawns = 0;

			while(pawn_ctx.tasks.empty() == false)
				pawn_ctx.tasks.pop();

			pawn_ctx.fin_i = 0;
			for(u16 i = 0; i < 256; i++)
				pawn_ctx.fin[i] = 0x00;
			
			inc_task_id = 0x00000000;
		};

		u32 King::push(task_callback_t callback, bool report) {
			if(initialized() == false) return (-1);

			u32 id = report
				? (inc_task_id++)
				: 0xffffffff;

			pawn_ctx.tasks.push(task_t {
				id,
				report,
				callback,
			});

			return id;
		};

		u32 King::push(task_callback_t callback) {
			return push(callback, false);
		};

		bool King::finished(u32 task_id) {
			// If it's blocked, wait 1 ms and check again.
			while(pawn_ctx.mtx_fin.try_lock() == false) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			};

			bool has_id = false;

			for(u16 i = 0; i < 256; i++) {
				if(pawn_ctx.fin[i] != task_id) continue;

				has_id = true;
				break;
			};

			pawn_ctx.mtx_fin.unlock();

			return has_id;
		};

		void King::remove_finished(u32 task_id) {
			// If it's blocked, wait 1 ms and check again.
			while(pawn_ctx.mtx_fin.try_lock() == false) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			};

			for(u16 i = 0; i < 256; i++) {
				if(pawn_ctx.fin[i] != task_id) continue;

				pawn_ctx.fin[i] = 0x00000000;
				break;
			};

			pawn_ctx.mtx_fin.unlock();
		};
	};
};

#endif
