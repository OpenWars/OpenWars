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
#ifndef __openwars__crypto__random__hpp__
#define __openwars__crypto__random__hpp__

#include "../nuclei.hpp"

namespace OpenWars {
	// (probably) Unsafe PRNG.
	class PRNG {
		private:
			u64	orig_seed,
				curr_seed,
				iteration;

		public:
			void init(void);

			void seed(u64 s);
			void seed_from_date(void);

			u32 random_u32(void);
			u64 random_u64(void);

			// A random float between from 0 to 1.
			f32 random_f32(void);
			f64 random_f64(void);

			u32 random_u32(u64 seq);
			u64 random_u64(u64 seq);

			// A random float between from 0 to 1.
			f32 random_f32(u64 seq);
			f64 random_f64(u64 seq);
	};
};

#endif
