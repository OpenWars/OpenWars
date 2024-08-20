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
#ifndef __openwars__crypto__sha1__hpp__
#define __openwars__crypto__sha1__hpp__

#include "../nuclei.hpp"

namespace OpenWars {
	namespace Crypto {
		// Implementation based on:
		// - [Brad Conte's "crypto-algorithms"](https://github.com/B-Con/crypto-algorithms).

		class SHA1 {
			private:
				//u8 *data;
				u8 data[64];
				u64 len;
				u64 bit_len;

				u32 state[5];
				u32 k[4];

				u8 hash[20];

				u32 rot_left(u32 a, u32 b);
				void transform(u8 *data);
			
			public:
				void init(void);
				void update(u8 *data, u64 len);
				u8 *digest(void);
		};
	};
};

#endif
