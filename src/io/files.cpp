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
#ifndef __openwars__io__files__cpp__
#define __openwars__io__files__cpp__

#include "files.hpp"
#include <cerrno>
#include <cstring>

namespace OpenWars {
	std::ios::openmode FileStream::__flags_to_std_openmode(u16 mode) {
		std::ios::openmode std_mode = std::ios::binary;

		if(mode & flags::in)
			std_mode |= std::ios::in;
			
		if(mode & flags::out)
			std_mode |= std::ios::out;
			
		return std_mode;
	};

	std::ios::seekdir FileStream::__flags_to_std_seekdir(u16 mode) {
		if(mode == flags::beg)
			return std::ios::beg;
		if(mode == flags::end)
			return std::ios::end;
			
		return std::ios::cur;
	};

	u64 FileStream::tellg(void) {
		return (u64)fs.tellg();
	};

	void FileStream::seekg(u64 pos) {
		fs.seekg(pos);
	};

	void FileStream::seekg(i64 off, seekdir dir) {
		fs.seekg(off, __flags_to_std_seekdir(dir));
	};
			
	u64 FileStream::tellp(void) {
		return (u64)fs.tellp();
	};

	void FileStream::seekp(u64 pos) {
		fs.seekp(pos);
	};

	void FileStream::seekp(i64 off, seekdir dir) {
		fs.seekp(off, __flags_to_std_seekdir(dir));
	};

	ErrorOr<void> FileStream::open(const char *path, u16 mode) {
		if(fs.is_open())
			return Error { "File stream is already open" };
		
		fs.open(path, __flags_to_std_openmode(mode));
		if(fs.fail())
			return Error { std::strerror(errno) };
			
		return Error { nullptr };
	};

	bool FileStream::is_open(void) {
		return fs.is_open();
	};
	
	void FileStream::close(void) {
		if(is_open())
			fs.close();
	};

	ErrorOr<void> FileStream::read(u8 *s, u64 n) {
		(void)fs.read((char *)s, n);

		if(fs.fail())
			return Error { std::strerror(errno) };
			
		return Error { nullptr };
	};

	ErrorOr<void> FileStream::write(u8 *s, u64 n) {
		(void)fs.write((char *)s, n);

		if(fs.fail())
			return Error { std::strerror(errno) };
			
		return Error { nullptr };
	};
};

#endif
