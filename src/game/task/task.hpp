#ifndef __openwars__task__task__hpp__
#define __openwars__task__task__hpp__

#include "../../defs.hpp"
#include <thread>
#include <queue>
#include <mutex>

namespace OpenWars {
	namespace Tasks {
		typedef int (*task_callback_t)(void);

		typedef struct {
			const char *success;
			const char *failure;
			task_callback_t callback;
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
				const char *err_str = nullptr;
				std::thread *thread_ptr = nullptr;

			public:
				~Pawn(void);

				const char *get_error(void);

				int init(pawn_ctx_t *pawn_ctx);
				bool initialized(void);
				void deinit(void);
		};

		class King {
			private:
				const char *err_str = nullptr;

				Pawn *pawns = nullptr;
				unsigned int number_of_pawns = 0;

				pawn_ctx_t pawn_ctx;
				bool fin_locked = false;

			public:
				~King(void);

				const char *get_error(void);
				unsigned int get_cpu_threads(void);

				bool initialized(void);
				int init_pawns(void);
				void deinit_pawns(void);

				int push(const char *success, const char *failure, task_callback_t callback);
				int push(task_callback_t callback);

				fin_queue_t * lock_fin(void);
				void unlock_fin(void);
				void wait(void);
		};
	};
};

#endif
