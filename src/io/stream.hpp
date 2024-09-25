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

#include "../nuclei.hpp"

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
			
			virtual i8 read(u8 *s, u64 n, const char **err) = 0;
			virtual i8 write(u8 *s, u64 n, const char **err) = 0;
			
			u8 readU8(const char **err);

			u16 readU16BE(const char **err);
			u32 readU32BE(const char **err);
			u64 readU64BE(const char **err);
			
			u16 readU16LE(const char **err);
			u32 readU32LE(const char **err);
			u64 readU64LE(const char **err);

			i8 readI8(const char **err);

			i16 readI16BE(const char **err);
			i32 readI32BE(const char **err);
			i64 readI64BE(const char **err);
			
			i16 readI16LE(const char **err);
			i32 readI32LE(const char **err);
			i64 readI64LE(const char **err);

			f32 readF32(const char **err);
			f64 readF64(const char **err);
			
			i8 writeU8(u8 value, const char **err);

			i8 writeU16BE(u16 value, const char **err);
			i8 writeU32BE(u32 value, const char **err);
			i8 writeU64BE(u64 value, const char **err);
			
			i8 writeU16LE(u16 value, const char **err);
			i8 writeU32LE(u32 value, const char **err);
			i8 writeU64LE(u64 value, const char **err);
			
			i8 writeI8(i8 value, const char **err);
			i8 writeI16LE(i16 value, const char **err);
			i8 writeI32LE(i32 value, const char **err);
			i8 writeI64LE(i64 value, const char **err);
			
			i8 writeF32(float value, const char **err);
			i8 writeF64(double value, const char **err);
	};
};

#endif
