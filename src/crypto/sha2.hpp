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
#ifndef __openwars__crypto__sha2__hpp__
#define __openwars__crypto__sha2__hpp__

#include "../defs.hpp"

namespace OpenWars {
	namespace Crypto {
		// Implementation based on:
		// - [Brad Conte's
		//   "crypto-algorithms"](https://github.com/B-Con/crypto-algorithms).

		class SHA2_256 {
			private:
				u8 *data;
				u64 len;
				u64 bit_len;

				u32 state[8];
				u8 hash[32];

				inline u32 rot_left(u32 a, u32 b);
				inline u32 rot_right(u32 a, u32 b);

				inline u32 ch(u32 x, u32 y, u32 z);
				inline u32 maj(u32 x, u32 y, u32 z);
				inline u32 ep0(u32 x);
				inline u32 ep1(u32 x);
				inline u32 sig0(u32 x);
				inline u32 sig1(u32 x);

				void transform(u8 *data);
			
			public:
				void init(void);
				void update(u8 *data, u64 len);
				u8 *digest(void);
		};
	};
};

#endif
