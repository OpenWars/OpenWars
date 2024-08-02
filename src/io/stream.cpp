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
#ifndef __openwars__io__stream__cpp__
#define __openwars__io__stream__cpp__

#include "stream.hpp"

namespace OpenWars {
	ErrorOr<u8> BaseStream::readU8(void) {
		u8 buff[1];
		ErrorOr<void> e = read(buff, 1);
		if(e.error) return Error { e.error };
		return buff[0];
	};
	
	ErrorOr<u16> BaseStream::readU16BE(void) {
		u8 buff[2];
		ErrorOr<void> e = read(buff, 2);
		if(e.error) return Error { e.error };
		
		return (
			(((u16)buff[0]) << 8) |
			((u16)buff[1])
		);
	};
	
	ErrorOr<u32> BaseStream::readU32BE(void) {
		u8 buff[4];
		ErrorOr<void> e = read(buff, 4);
		if(e.error) return Error { e.error };
		
		return (
			(((u32)buff[0]) << 24) |
			(((u32)buff[1]) << 16) |
			(((u32)buff[2]) << 8) |
			((u32)buff[3])
		);
	};
	
	ErrorOr<u64> BaseStream::readU64BE(void) {
		u8 buff[8];
		ErrorOr<void> e = read(buff, 8);
		if(e.error) return Error { e.error };
		
		return (
			(((u64)buff[0]) << 56) |
			(((u64)buff[1]) << 48) |
			(((u64)buff[2]) << 40) |
			(((u64)buff[3]) << 32) |
			(((u64)buff[4]) << 24) |
			(((u64)buff[5]) << 16) |
			(((u64)buff[6]) << 8) |
			((u64)buff[7])
		);
	};
	
	ErrorOr<u16> BaseStream::readU16LE(void) {
		ErrorOr<u16> e = readU16BE();
		if(e.error) return e;
		
		u16 num = e.value();
		num = (
			(num << 8) |
			(num >> 8)
		);
		
		return num;
	};
	
	ErrorOr<u32> BaseStream::readU32LE(void) {
		ErrorOr<u32> e = readU32BE();
		if(e.error) return Error { e.error };
		
		u32 num = e.value();
		
		return (
			(num >> 24) |
			((num << 8) & 0xff0000) |
			((num >> 8) & 0xff00) |
			((num << 24) & 0xff000000)
		);
	};
	
	ErrorOr<u64> BaseStream::readU64LE(void) {
		ErrorOr<u64> e = readU64BE();
		if(e.error) return Error { e.error };
		
		u64 num = e.value();
		
		return (
			((num >> 56)	& 0x00000000000000ff) |
			((num >> 40)	& 0x000000000000ff00) |
			((num >> 24)	& 0x0000000000ff0000) |
			((num >> 8)		& 0x00000000ff000000) |
			((num << 8)		& 0x000000ff00000000) |
			((num << 24)	& 0x0000ff0000000000) |
			((num << 40)	& 0x00ff000000000000) |
			((num << 56)	& 0xff00000000000000)
		);
	};
	
	ErrorOr<i8> BaseStream::readI8(void) {
		ErrorOr<u8> e = readU8();
		if(e.error) return Error { e.error };

		u8 x[1] = { e.value() };
		return (*(i8 *)&x);
	};
	
	ErrorOr<i16> BaseStream::readI16BE(void) {
		ErrorOr<u16> e = readU16BE();
		if(e.error) return Error { e.error };

		u16 x[1] = { e.value() };
		return (*(i16 *)&x);
	};

	ErrorOr<i32> BaseStream::readI32BE(void) {
		ErrorOr<u32> e = readU32BE();
		if(e.error) return Error { e.error };

		u32 x[1] = { e.value() };
		return (*(i32 *)&x);
	};

	ErrorOr<i64> BaseStream::readI64BE(void) {
		ErrorOr<u64> e = readU64BE();
		if(e.error) return Error { e.error };

		u64 x[1] = { e.value() };
		return (*(i64 *)&x);
	};

	ErrorOr<i16> BaseStream::readI16LE(void) {
		ErrorOr<u16> e = readU16LE();
		if(e.error) return Error { e.error };

		u16 x[1] = { e.value() };
		return (*(i16 *)&x);
	};

	ErrorOr<i32> BaseStream::readI32LE(void) {
		ErrorOr<u32> e = readU32LE();
		if(e.error) return Error { e.error };

		u32 x[1] = { e.value() };
		return (*(i32 *)&x);
	};

	ErrorOr<i64> BaseStream::readI64LE(void) {
		ErrorOr<u64> e = readU64LE();
		if(e.error) return Error { e.error };

		u64 x[1] = { e.value() };
		return (*(i64 *)&x);
	};

	ErrorOr<void> BaseStream::writeU8(u8 value) {
		u8 buff[1] = {
			(u8)value
		};
		
		ErrorOr<void> e = write(buff, 1);
		if(e.error) return Error { e.error };
		return Error { nullptr };
	};
	
	ErrorOr<void> BaseStream::writeU16BE(u16 value) {
		u8 buff[2] = {
			(u8)((value >> 8) & 0xff),
			(u8)(value & 0xff)
		};
		
		ErrorOr<void> e = write(buff, 2);
		if(e.error) return Error { e.error };
		return Error { nullptr };
	};
	
	ErrorOr<void> BaseStream::writeU32BE(u32 value) {
		u8 buff[4] = {
			(u8)((value >> 24) & 0xff),
			(u8)((value >> 16) & 0xff),
			(u8)((value >> 8) & 0xff),
			(u8)(value & 0xff)
		};
		
		ErrorOr<void> e = write(buff, 4);
		if(e.error) return Error { e.error };
		return Error { nullptr };
	};
	
	ErrorOr<void> BaseStream::writeU64BE(u64 value) {
		u8 buff[8] = {
			(u8)((value >> 56) & 0xff),
			(u8)((value >> 48) & 0xff),
			(u8)((value >> 40) & 0xff),
			(u8)((value >> 32) & 0xff),
			(u8)((value >> 24) & 0xff),
			(u8)((value >> 16) & 0xff),
			(u8)((value >> 8) & 0xff),
			(u8)(value & 0xff)
		};
		
		ErrorOr<void> e = write(buff, 8);
		if(e.error) return Error { e.error };
		return Error { nullptr };
	};
	
	ErrorOr<void> BaseStream::writeU16LE(u16 value) {
		u8 buff[2] = {
			(u8)(value & 0xff),
			(u8)((value >> 8) & 0xff)
		};
		
		ErrorOr<void> e = write(buff, 2);
		if(e.error) return Error { e.error };
		return Error { nullptr };
	};
	
	ErrorOr<void> BaseStream::writeU32LE(u32 value) {
		u8 buff[4] = {
			(u8)(value & 0xff),
			(u8)((value >> 8) & 0xff),
			(u8)((value >> 16) & 0xff),
			(u8)((value >> 24) & 0xff)
		};
		
		ErrorOr<void> e = write(buff, 4);
		if(e.error) return Error { e.error };
		return Error { nullptr };
	};
	
	ErrorOr<void> BaseStream::writeU64LE(u64 value) {
		u8 buff[8] = {
			(u8)(value & 0xff),
			(u8)((value >> 8) & 0xff),
			(u8)((value >> 16) & 0xff),
			(u8)((value >> 24) & 0xff),
			(u8)((value >> 32) & 0xff),
			(u8)((value >> 40) & 0xff),
			(u8)((value >> 48) & 0xff),
			(u8)((value >> 56) & 0xff)
		};
		
		ErrorOr<void> e = write(buff, 8);
		if(e.error) return Error { e.error };
		return Error { nullptr };
	};
	
	// [TODO] : Pasar i8->u8 sin pérdidas.
	ErrorOr<void> BaseStream::writeI8(i8 value) {
		return writeU8(value);
	};

	ErrorOr<void> BaseStream::writeI16LE(i16 value) {
		return writeU16LE(value);
	};

	ErrorOr<void> BaseStream::writeI32LE(i32 value) {
		return writeU32LE(value);
	};

	ErrorOr<void> BaseStream::writeI64LE(i64 value) {
		return writeU64LE(value);
	};
	
	ErrorOr<float> BaseStream::readF32(void) {
		ErrorOr<u32> raw_value = readU32BE();
		if(raw_value.error) return Error { raw_value.error };
		
		u32 v32 = raw_value.value();
		u32 *v32p = (&v32);
		float value = *(float *)v32p;
		
		return value;
	};
	
	ErrorOr<double> BaseStream::readF64(void) {
		ErrorOr<u64> raw_value = readU64BE();
		if(raw_value.error) return Error { raw_value.error };
		
		u64 v64 = raw_value.value();
		u64 *v64p = (&v64);
		double value = *(double *)v64p;
		
		return value;
	};
	
	ErrorOr<void> BaseStream::writeF32(float value) {
		u32 *raw_value = (u32 *)(&value);
		return writeU32BE(*raw_value);
	};
	
	ErrorOr<void> BaseStream::writeF64(double value) {
		u64 *raw_value = (u64 *)(&value);
		return writeU64BE(*raw_value);
	};
};

#endif
