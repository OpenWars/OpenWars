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

		typedef struct _pawn_ctx_t {
			std::mutex mtx_tasks;
			std::mutex mtx_fin;

			std::queue<task_t> tasks;

			u32 fin[256] = { 0x00000000 };
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

				u32 inc_task_id = 0x00000000;

			public:
				~King(void);

				const char *get_error(void);
				unsigned int get_cpu_threads(void);

				bool initialized(void);
				int init_pawns(void);
				void deinit_pawns(void);

				u32 push(task_callback_t callback, bool report);
				u32 push(task_callback_t callback);

				bool finished(u32 task_id);
				void remove_finished(u32 task_id);
		};
	};
};

#endif
