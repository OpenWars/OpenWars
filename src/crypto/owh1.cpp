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
#ifndef __openwars__crypto__wha1__cpp__
#define __openwars__crypto__wha1__cpp__

#include "owh1.hpp"

#define OWH1_ROUNDS 10

namespace OpenWars {
	namespace Crypto {
		inline u32 OWH1_128::rot_left(u32 x, u32 s) {
			return ((x << s) | (x >> (32 - s)));
		};

		inline u32 OWH1_128::rot_right(u32 x, u32 s) {
			return ((x >> s) | (x << (32 - s)));
		};

		inline u32 OWH1_128::adc(u32 x, u32 y) {
			u32 z = x + y;
			return z + (z < x);
		};

		void OWH1_128::round(u32 &a, u32 &b, u32 &c, u32 &d) {
			a = adc(a, (d ^ 5));
			b = adc(b, (c ^ 11));
			c = adc(c, (d ^ 17));
			d = adc(d, (c ^ 23));

			b = rot_left(adc(b, c), 29);
			c = rot_left(adc(c, d), 19);
			d = rot_left(adc(d, a), 13);
			a = rot_left(adc(a, d), 7);
		};

		void OWH1_128::transform(void) {
			u32 *b = (u32 *)(this->block);
			u8 rounds = OWH1_ROUNDS + (u8)(state[4] & 0x3);

			for(u8 i = 0; i < rounds; i++) {
				round(b[0], state[0], b[1], state[2]);
				round(b[2], state[4], b[3], state[6]);
				round(b[0], state[1], b[1], state[3]);
				round(b[2], state[5], b[3], state[7]);
				round(state[0], state[4], state[1], state[5]);
				round(state[2], state[6], state[3], state[7]);
				round(state[0], state[1], state[2], state[3]);
				round(state[4], state[5], state[6], state[7]);
			};
		};

		void OWH1_128::init(u64 nonce_a, u64 nonce_b) {
			state[0] = 0x94fe680b;
			state[1] = (0xd2277859 ^ (u32)(nonce_a >> 32));
			state[2] = 0xd59c944b;
			state[3] = (0xc6b2041b ^ (u32)nonce_a);
			state[4] = 0xe13d4527;
			state[5] = (0xf6b9d3dd ^ (u32)(nonce_b >> 32));
			state[6] = 0x45afd531;
			state[7] = (0x7ddb19e5 ^ (u32)nonce_b);

			for(u8 i = 0; i < 16; i++)
				block[i] = 0x00;

			block_len = 0,
			data_len = 0;
		};

		void OWH1_128::init(void) {
			init(0xabbaabbaabbaabba, 0x5aa55aa55aa55aa5);
		};

		void OWH1_128::update(u8 *data, u64 len) {
			for(u64 i = 0; i < len; i++) {
				block[block_len++] = data[i];
				data_len++;

				if(block_len == 16) {
					transform();
					block_len = 0;
				}
			};
		};

		u8 *OWH1_128::digest(void) {
			u8 data[8] = {
				(u8)(data_len >> 56),
				(u8)(data_len >> 48),
				(u8)(data_len >> 40),
				(u8)(data_len >> 32),
				(u8)(data_len >> 24),
				(u8)(data_len >> 16),
				(u8)(data_len >> 8),
				(u8)data_len,
			};

			update(data, 8);
			if(block_len != 0)
				transform();

			state[0] = adc(state[0], rot_left(state[7], 11));
			state[1] = adc(state[1], rot_right(state[6], 13));
			state[2] = adc(state[2], rot_left(state[5], 17));
			state[3] = adc(state[3], rot_right(state[4], 23));

			for(u8 i = 0, j = 0; i < 16; i += 4, j++) {
				hash[i | 0x0] = (u8)(state[j] >> 24);
				hash[i | 0x1] = (u8)(state[j] >> 16);
				hash[i | 0x2] = (u8)(state[j] >> 8);
				hash[i | 0x3] = (u8)state[j];
			};

			return hash;
		};
	};
};

#endif
