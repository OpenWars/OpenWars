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
#ifndef __openwars__crypto__sha1__cpp__
#define __openwars__crypto__sha1__cpp__

#include "sha1.hpp"

namespace OpenWars {
	namespace Crypto {
		inline u32 SHA1::rot_left(u32 a, u32 b) {
			return ((a << b) | (a >> (32 - b)));
		};

		void SHA1::transform(u8 *data) {
			u32 a, b, c, d, e, i, j, t, m[80];

			for(i = 0, j = 0; i < 16; i++, j += 4)
				m[i] =	((data[j] << 24) +
						(data[j + 1] << 16) +
						(data[j + 2] << 8) +
						data[j + 3]);

			for(; i < 80; i++) {
				m[i] =	(m[i - 3] ^
						m[i - 8] ^
						m[i - 14] ^
						m[i - 16]);
				
				m[i] = ((m[i] << 1) | (m[i] >> 31));
			};

			a = state[0],
			b = state[1],
			c = state[2],
			d = state[3],
			e = state[4];

			for(i = 0; i < 20; i++) {
				t =	(rot_left(a, 5) + ((b & c) ^ ((~b) & d)) + e * k[0] + m[i]);
				e = d;
				d = c;
				c = rot_left(b, 30);
				b = a;
				a = t;
			};

			for(; i < 40; i++) {
				t = (rot_left(a, 5) + (b ^ c ^ d) + e + k[1] + m[i]);
				e = d;
				d = c;
				c = rot_left(b, 30);
				b = a;
				a = t;
			};

			for(; i < 60; i++) {
				t =	(rot_left(a, 5) +
					((b & c) ^ (b & d) ^ (c & d)) +
					e + k[2] + m[i]);
				
				e = d;
				d = c;
				c = rot_left(b, 30);
				b = a;
				a = t;
			};

			for(; i < 80; i++) {
				t = (rot_left(a, 5) + (b ^ c ^d) + e + k[3] + m[i]);
				e = d;
				d = c;
				c = rot_left(b, 30);
				b = a;
				a = t;
			};

			state[0] += a;
			state[1] += b;
			state[2] += c;
			state[3] += d;
			state[4] += e;
		};
		
		void SHA1::init(void) {
			data = nullptr,
			len = 0,
			bit_len = 0,
			// Little-Endian 0123456789...
			state[0] = 0x67452301,
			state[1] = 0xefcdab89,
			state[2] = 0x98badcfe,
			state[3] = 0x10325476,
			state[4] = 0xc3d2e1f0;
			// Square root of 2, 3, 5 and 10.
			k[0] = 0x5a827999,
			k[1] = 0x6ed9eba1,
			k[2] = 0x8f1bbcdc,
			k[3] = 0xca62c1d6;
		};

		void SHA1::update(u8 *data, u64 len) {
			for(u64 i = 0; i < len; i++) {
				this->data[this->len] = data[i];
				this->len++;

				if(this->len == 64) {
					transform(this->data);
					this->bit_len += 512;
					this->len = 0;
				}
			};
		};

		u8 *SHA1::digest(void) {
			u32 i = len;

			// Pad whatever data is left in the buffer.
			if(len < 56) {
				data[i++] = 0x00;
				while(i < 56)
					data[i++] = 0x00;
			} else {
				data[i++] = 0x00;
				while(i < 64)
					data[i++] = 0x00;

				transform(data);

				for(i = 0; i < 56; i++)
					data[i] = 0x00;
			}

			// Append to the padding the total message's length (in bits), and
			// transform.
			bit_len += (len << 3);
			data[63] = bit_len;
			data[62] = (bit_len >> 8);
			data[61] = (bit_len >> 16);
			data[60] = (bit_len >> 24);
			data[59] = (bit_len >> 32);
			data[58] = (bit_len >> 40);
			data[57] = (bit_len >> 48);
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
			};

			return hash;
		};
	};
};

#endif
