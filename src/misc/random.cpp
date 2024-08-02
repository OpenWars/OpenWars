/**
 *
   ___                __        __
  / _ \ _ __   ___ _ _\ \      / /_ _ _ __ ___
 | | | | '_ \ / _ \ '_ \ \ /\ / / _` | '__/ __|
 | |_| | |_) |  __/ | | \ V  V / (_| | |  \__ \
  \___/| .__/ \___|_| |_|\_/\_/ \__,_|_|  |___/
       |_|

Copyright (C) 2024 OpenWars Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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
