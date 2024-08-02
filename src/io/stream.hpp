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
#ifndef __openwars__io__stream__hpp__
#define __openwars__io__stream__hpp__

#include "../defs.hpp"

namespace OpenWars {
	typedef enum {
		beg = 1,
		cur = 2,
		end = 3,
	} seekdir;

	class BaseStream {
		public:
			virtual u64 tellg(void) = 0;
			virtual void seekg(u64 pos) = 0;
			virtual void seekg(i64 off, seekdir dir) = 0;
			
			virtual u64 tellp(void) = 0;
			virtual void seekp(u64 pos) = 0;
			virtual void seekp(i64 off, seekdir dir) = 0;
			
			virtual ErrorOr<void> read(u8 *s, u64 n) = 0;
			virtual ErrorOr<void> write(u8 *s, u64 n) = 0;
			
			void skipg(i64 pos);
			void skipp(i64 pos);
			
			ErrorOr<u8> readU8(void);

			ErrorOr<u16> readU16BE(void);
			ErrorOr<u32> readU32BE(void);
			ErrorOr<u64> readU64BE(void);
			
			ErrorOr<u16> readU16LE(void);
			ErrorOr<u32> readU32LE(void);
			ErrorOr<u64> readU64LE(void);

			ErrorOr<i8> readI8(void);

			ErrorOr<i16> readI16BE(void);
			ErrorOr<i32> readI32BE(void);
			ErrorOr<i64> readI64BE(void);
			
			ErrorOr<i16> readI16LE(void);
			ErrorOr<i32> readI32LE(void);
			ErrorOr<i64> readI64LE(void);
			
			ErrorOr<void> writeU8(u8 value);

			ErrorOr<void> writeU16BE(u16 value);
			ErrorOr<void> writeU32BE(u32 value);
			ErrorOr<void> writeU64BE(u64 value);
			
			ErrorOr<void> writeU16LE(u16 value);
			ErrorOr<void> writeU32LE(u32 value);
			ErrorOr<void> writeU64LE(u64 value);
			
			ErrorOr<void> writeI8(i8 value);
			ErrorOr<void> writeI16LE(i16 value);
			ErrorOr<void> writeI32LE(i32 value);
			ErrorOr<void> writeI64LE(i64 value);
			
			ErrorOr<float> readF32(void);
			ErrorOr<double> readF64(void);
			
			ErrorOr<void> writeF32(float value);
			ErrorOr<void> writeF64(double value);
	};
};

#endif
