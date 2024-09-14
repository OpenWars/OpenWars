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
#ifndef __openwars__crypto__sha2__cpp__
#define __openwars__crypto__sha2__cpp__

#include "sha2.hpp"

namespace OpenWars {
	namespace Crypto {
		const u32 SHA2_256_K[64] = {
			0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
			0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
			0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
			0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
			0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
			0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
			0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
			0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
			0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
			0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
			0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
			0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
			0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
			0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
			0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
		};

		inline u32 SHA2_256::rot_left(u32 a, u32 b) {
			return ((a << b) | (a >> (32 - b)));
		};

		inline u32 SHA2_256::rot_right(u32 a, u32 b) {
			return ((a >> b) | (a << (32 - b)));
		};

		inline u32 SHA2_256::ch(u32 x, u32 y, u32 z) {
			return ((x & y) ^ ((~x) & z));
		};

		inline u32 SHA2_256::maj(u32 x, u32 y, u32 z) {
			return ((x & y) ^ (x & z) ^ (y & z));
		};

		inline u32 SHA2_256::ep0(u32 x) {
			return	(rot_right(x, 2) ^
					rot_right(x, 13) ^
					rot_right(x, 22));
		};

		inline u32 SHA2_256::ep1(u32 x) {
			return	(rot_right(x, 6) ^
					rot_right(x, 11) ^
					rot_right(x, 25));
		};

		inline u32 SHA2_256::sig0(u32 x) {
			return	(rot_right(x, 7) ^
					rot_right(x, 18) ^
					(x >> 3));
		};

		inline u32 SHA2_256::sig1(u32 x) {
			return	(rot_right(x, 17) ^
					rot_right(x, 19) ^
					(x >> 10));
		};

		void SHA2_256::transform(u8 *data) {
			u32 a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

			for(i = 0, j = 0; i < 16; i++, j += 4)
				m[i] =	((data[j] << 24) |
						(data[j + 1] << 16) |
						(data[j + 2] << 8) |
						data[j + 3]);
			
			for(; i < 64; i++)
				m[i] =	(sig1(m[i - 2]) +
						m[i - 7] +
						sig0(m[i - 15]) +
						m[i - 16]);
			
			a = state[0],
			b = state[1],
			c = state[2],
			d = state[3],
			e = state[4],
			f = state[5],
			g = state[6],
			h = state[7];

			for(i = 0; i < 64; i++) {
				t1 = h + ep1(e) + ch(e, f, g) + SHA2_256_K[i] + m[i];
				t2 = ep0(a) + maj(a, b, c);
				h = g;
				g = f;
				f = e;
				e = d + t1;
				d = c;
				c = b;
				b = a;
				a = t1 + t2;
			}

			state[0] += a;
			state[1] += b;
			state[2] += c;
			state[3] += d;
			state[4] += e;
			state[5] += f;
			state[6] += g;
			state[7] += h;
		};

		void SHA2_256::init(void) {
			len = 0,
			bit_len = 0,
			state[0] = 0x6a09e667,
			state[1] = 0xbb67ae85,
			state[2] = 0x3c6ef372,
			state[3] = 0xa54ff53a,
			state[4] = 0x510e527f,
			state[5] = 0x9b05688c,
			state[6] = 0x1f83d9ab,
			state[7] = 0x5be0cd19;
		};

		void SHA2_256::update(u8 *data, u64 len) {
			for(u32 i = 0; i < len; i++) {
				this->data[this->len] = data[i];
				this->len++;

				if(this->len == 64) {
					transform(this->data);
					bit_len += 512;
					len = 0;
				}
			};
		};

		u8 *SHA2_256::digest(void) {
			u32 i = len;

			// Pad whatever data is left in the buffer.
			if(len < 56) {
				data[i++] = 0x00;
				while(i < 56)
					data[i++] = 0x00;
			} else {
				data[i++] = 0x80;
				while(i < 64)
					data[i++] = 0x00;
				
				transform(data);

				for(i = 0; i < 56; i++)
					data[i] = 0;
			}

			// Append to the padding the total message's length (in bits), and
			// transform.
			bit_len += (len * 8),
			data[63] = bit_len,
			data[62] = (bit_len >> 8),
			data[61] = (bit_len >> 16),
			data[60] = (bit_len >> 24),
			data[59] = (bit_len >> 32),
			data[58] = (bit_len >> 40),
			data[57] = (bit_len >> 48),
			data[56] = (bit_len >> 56);

			transform(data);

			// Since this implementation uses Little-Endian, and MD uses
			// Big-Endian, we will reverse all the bytes when copying the final
			// state to the output hash.
			for(i = 0; i < 4; i++) {
				hash[i]			= (u8)(state[0] >> (24 - (i << 3)));
				hash[4 + i]		= (u8)(state[1] >> (24 - (i << 3)));
				hash[8 + i]		= (u8)(state[2] >> (24 - (i << 3)));
				hash[12 + i]	= (u8)(state[3] >> (24 - (i << 3)));
				hash[16 + i]	= (u8)(state[4] >> (24 - (i << 3)));
				hash[20 + i]	= (u8)(state[5] >> (24 - (i << 3)));
				hash[24 + i]	= (u8)(state[6] >> (24 - (i << 3)));
				hash[28 + i]	= (u8)(state[7] >> (24 - (i << 3)));
			};

			return hash;
		};
	};
};

#endif
