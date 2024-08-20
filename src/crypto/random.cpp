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
#ifndef __openwars__crypto__random__cpp__
#define __openwars__crypto__random__cpp__

#include "random.hpp"
#include "../crypto/sha1.hpp"
#include <chrono>

namespace OpenWars {
	void PRNG::init(void) {
		curr_seed = orig_seed = 0xabababababababab;
		iteration = 0;
	};

	void PRNG::seed(u64 s) {
		curr_seed = orig_seed = s;
	};

	void PRNG::seed_from_date(void) {
		std::chrono::time_point t = std::chrono::system_clock::now();
		std::chrono::duration u = t.time_since_epoch();

		u64 c0 = (u64)std::chrono::duration_cast<std::chrono::seconds>(u).count();
		u64 c1 = (u64)std::chrono::duration_cast<std::chrono::milliseconds>(u).count();
		u64 c2 = (u64)std::chrono::duration_cast<std::chrono::nanoseconds>(u).count();

		// Random prime numbers.
		c0 += 0x18d245eb0510cb11;
		c1 += 0x0510cb114d8ada37;
		c2 += 0x4d8ada3718d245eb;

		c0 = (c0 ^ c1 ^ c2);

		orig_seed = curr_seed = c0;
		iteration = 0;
	};

	u32 PRNG::random_u32(void) {
		u8 data[] = {
			(u8)(orig_seed >> 56), (u8)(orig_seed >> 48),
			(u8)(orig_seed >> 40), (u8)(orig_seed >> 32),
			(u8)(orig_seed >> 24), (u8)(orig_seed >> 16),
			(u8)(orig_seed >> 8), (u8)orig_seed,

			(u8)(curr_seed >> 56), (u8)(curr_seed >> 48),
			(u8)(curr_seed >> 40), (u8)(curr_seed >> 32),
			(u8)(curr_seed >> 24), (u8)(curr_seed >> 16),
			(u8)(curr_seed >> 8), (u8)curr_seed,

			(u8)(iteration >> 56), (u8)(iteration >> 48),
			(u8)(iteration >> 40), (u8)(iteration >> 32),
			(u8)(iteration >> 24), (u8)(iteration >> 16),
			(u8)(iteration >> 8), (u8)iteration,
		};

		Crypto::SHA1 sha1;
		sha1.init();
		sha1.update(data, 24);
		u8 *d = sha1.digest();

		u32 v =	((((u32)d[0]) << 24) |
				(((u32)d[1]) << 16) |
				(((u32)d[2]) << 8) |
				((u32)d[3]));
		
		iteration++;

		curr_seed =	((((u64)d[8]) << 56) |
					(((u64)d[9]) << 48) |
					(((u64)d[10]) << 40) |
					(((u64)d[11]) << 32) |
					(((u64)d[12]) << 24) |
					(((u64)d[13]) << 16) |
					(((u64)d[14]) << 8) |
					((u64)d[15]));

		return v;
	};

	u64 PRNG::random_u64(void) {
		u8 data[] = {
			(u8)(orig_seed >> 56), (u8)(orig_seed >> 48),
			(u8)(orig_seed >> 40), (u8)(orig_seed >> 32),
			(u8)(orig_seed >> 24), (u8)(orig_seed >> 16),
			(u8)(orig_seed >> 8), (u8)orig_seed,

			(u8)(curr_seed >> 56), (u8)(curr_seed >> 48),
			(u8)(curr_seed >> 40), (u8)(curr_seed >> 32),
			(u8)(curr_seed >> 24), (u8)(curr_seed >> 16),
			(u8)(curr_seed >> 8), (u8)curr_seed,

			(u8)(iteration >> 56), (u8)(iteration >> 48),
			(u8)(iteration >> 40), (u8)(iteration >> 32),
			(u8)(iteration >> 24), (u8)(iteration >> 16),
			(u8)(iteration >> 8), (u8)iteration,
		};

		Crypto::SHA1 sha1;
		sha1.init();
		sha1.update(data, 24);
		u8 *d = sha1.digest();

		u64 v =	((((u64)d[0]) << 56) |
				(((u64)d[1]) << 48) |
				(((u64)d[2]) << 40) |
				(((u64)d[3]) << 32) |
				(((u64)d[4]) << 24) |
				(((u64)d[5]) << 16) |
				(((u64)d[6]) << 8) |
				((u64)d[7]));
		
		iteration++;

		curr_seed =	((((u64)d[8]) << 56) |
					(((u64)d[9]) << 48) |
					(((u64)d[10]) << 40) |
					(((u64)d[11]) << 32) |
					(((u64)d[12]) << 24) |
					(((u64)d[13]) << 16) |
					(((u64)d[14]) << 8) |
					((u64)d[15]));

		return v;
	};

	f32 PRNG::random_f32(void) {
		u32 i = (random_u32() & 0x7fffff);
		f32 f = (((f32)i) / (f32)0x800000);

		return f;
	};

	f64 PRNG::random_f64(void) {
		u64 i = (random_u64() & 0xfffffffffffff);
		f64 f = (((f64)i) / (f64)0x10000000000000);

		return f;
	};

	u32 PRNG::random_u32(u64 seq) {
		u8 data[] = {
			(u8)(orig_seed >> 56), (u8)(orig_seed >> 48),
			(u8)(orig_seed >> 40), (u8)(orig_seed >> 32),
			(u8)(orig_seed >> 24), (u8)(orig_seed >> 16),
			(u8)(orig_seed >> 8), (u8)orig_seed,

			(u8)(seq >> 56), (u8)(seq >> 48),
			(u8)(seq >> 40), (u8)(seq >> 32),
			(u8)(seq >> 24), (u8)(seq >> 16),
			(u8)(seq >> 8), (u8)seq,
		};

		Crypto::SHA1 sha1;
		sha1.init();
		sha1.update(data, 16);
		u8 *d = sha1.digest();

		u32 v =	((((u32)d[16]) << 24) |
				(((u32)d[18]) << 16) |
				(((u32)d[17]) << 8) |
				((u32)d[19]));
		
		// Pi (F32 as U32).
		v ^= 0x40490fdb;
		
		return v;
	};

	u64 PRNG::random_u64(u64 seq) {
		u8 data[] = {
			(u8)(orig_seed >> 56), (u8)(orig_seed >> 48),
			(u8)(orig_seed >> 40), (u8)(orig_seed >> 32),
			(u8)(orig_seed >> 24), (u8)(orig_seed >> 16),
			(u8)(orig_seed >> 8), (u8)orig_seed,

			(u8)(seq >> 56), (u8)(seq >> 48),
			(u8)(seq >> 40), (u8)(seq >> 32),
			(u8)(seq >> 24), (u8)(seq >> 16),
			(u8)(seq >> 8), (u8)seq,
		};

		Crypto::SHA1 sha1;
		sha1.init();
		sha1.update(data, 16);
		u8 *d = sha1.digest();

		u64 v =	((((u64)d[16]) << 56) |
				(((u64)d[17]) << 48) |
				(((u64)d[18]) << 40) |
				(((u64)d[19]) << 32) |
				(((u64)d[20]) << 24) |
				(((u64)d[21]) << 16) |
				(((u64)d[22]) << 8) |
				((u64)d[23]));
		
		// Pi (F64 as U64).
		v ^= 0x400921fb54442d18;
		
		return v;
	};

	f32 PRNG::random_f32(u64 seq) {
		u32 i = (random_u32(seq) & 0x7fffff);
		f32 f = (((f32)i) / (f32)0x800000);

		return f;
	};

	f64 PRNG::random_f64(u64 seq) {
		u64 i = (random_u64(seq) & 0xfffffffffffff);
		f64 f = (((f64)i) / (f64)0x10000000000000);

		return f;
	};
};

#endif
