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
#ifndef __openwars__crypto__owh1__hpp__
#define __openwars__crypto__owh1__hpp__

#include "../nuclei.hpp"

namespace OpenWars {
	namespace Crypto {
		// A custom hash algorithm for OpenWars.
		class OWH1_128 {
			private:
				u32	state[8];
				u8	block[16];

				u8	block_len;
				u64	data_len;

				u8	hash[16];

				inline u32 rot_left(u32 x, u32 s);
				inline u32 rot_right(u32 x, u32 s);
				inline u32 adc(u32 x, u32 y);

				void round(u32 &a, u32 &b, u32 &c, u32 &d);
				void transform(void);

			public:
				void init(u64 nonce_a, u64 nonce_b);
				void init(void);
				void update(u8 *data, u64 len);
				u8 *digest(void);
		};
	};
};

#endif
