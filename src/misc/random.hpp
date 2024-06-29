#ifndef __openwars__random__hpp__
#define __openwars__random__hpp__

#include "../defs.hpp"

namespace OpenWars {
	// Unsafe PRNG.
	class Random {
		private:
			u32 orig_seed;
			u32 curr_seed;
			u32 iteration;

		public:
			void init(void);

			void seed(u32 seed);
			void seed_from_date(void);

			u32 random_u32(void);
			// A random float between from 0 to 1.
			f32 random_f32(void);

			u32 random_u32(u32 seq);
			// A random float between from 0 to 1.
			f32 random_f32(u32 seq);
	};
};

#endif
