#ifndef __openwars__random__cpp__
#define __openwars__random__cpp__

#include "random.hpp"
#include <chrono>

namespace OpenWars {
	void Random::init(void) {
		orig_seed = 0xaaaaaaaa;
		curr_seed = orig_seed;
		iteration = 0;
	};

	void Random::seed(u32 seed) {
		orig_seed = seed;
		curr_seed = seed;
	};

	void Random::seed_from_date(void) {
		std::chrono::time_point t = std::chrono::system_clock::now();
		std::chrono::duration u = t.time_since_epoch();

		u32 c0 = (u32)std::chrono::duration_cast<std::chrono::seconds>(u).count();
		u32 c1 = (u32)std::chrono::duration_cast<std::chrono::milliseconds>(u).count();
		u32 c2 = (u32)std::chrono::duration_cast<std::chrono::nanoseconds>(u).count();

		// Random prime numbers.
		c0 += 0x18d245eb;
		c1 += 0x0510cb11;
		c2 += 0x4d8ada37;

		curr_seed = ((c0 ^ c1) ^ c2);
		orig_seed = curr_seed;
	};

	u32 Random::random_u32(void) {
		u32 x = (curr_seed | 0x3);
		x ^= ((curr_seed << 13) ^ (curr_seed >> 7));
		x ^= ((iteration << 19) ^ (iteration >> 11));
		x ^= ((curr_seed << 23) ^ (curr_seed >> 17));

		curr_seed = (x ^ 0xa5a5a5a5);
		iteration++;

		return x;
	};

	f32 Random::random_f32(void) {
		f32 x = (f32)(random_u32() & 0x7fffff);
		f32 y = (x / 0x800000);

		return y;
	};

	u32 Random::random_u32(u32 seq) {
		u32 x = (seq | 0x3);
		x ^= ((orig_seed << 13) ^ (orig_seed >> 7));
		x ^= (((seq << 19) ^ (seq >> 11)) << 15);
		// Pi (F32 as U32).
		x ^= 0x40490fdb;

		return x;
	};

	f32 Random::random_f32(u32 seq) {
		f32 x = (f32)(random_u32(seq) & 0x7fffff);
		f32 y = (x / 0x800000);

		return y;
	};
};

#endif
