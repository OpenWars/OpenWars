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
#include <fstream>
#include <cerrno>
#include <cstring>
#include <filesystem>

namespace OpenWars {
	int create_directories(const char *path, const char **err) {
		if(*err != nullptr)
			return -1;

		try {
			std::filesystem::create_directories(path);
		} catch(...) {
			*err = "Couldn't create the directories";
			return -1;
		};

		return 0;
	};

	std::ios::openmode i_fs_openmode_to_std(unsigned int mode) {
		std::ios::openmode std_mode = std::ios::binary;

		if(mode & OpenWars::in)
			std_mode |= std::ios::in;
		if(mode & OpenWars::out)
			std_mode |= std::ios::out;

		return std_mode;
	};

	std::ios::seekdir i_fs_seekdir_to_std(unsigned int mode) {
		if((mode & 0xc0) == OpenWars::beg)
			return std::ios::beg;
		if((mode & 0xc0) == OpenWars::end)
			return std::ios::end;

		return std::ios::cur;
	};

	u64 FileStream::tellg(void) {
		return (u64)i_fs.tellg();
	};

	void FileStream::seekg(u64 pos) {
		i_fs.seekg(pos);
	};

	void FileStream::seekg(i64 off, seekdir dir) {
		i_fs.seekg(off, i_fs_seekdir_to_std(dir));
	};
			
	u64 FileStream::tellp(void) {
		return (u64)i_fs.tellp();
	};

	void FileStream::seekp(u64 pos) {
		i_fs.seekp(pos);
	};

	void FileStream::seekp(i64 off, seekdir dir) {
		i_fs.seekp(off, i_fs_seekdir_to_std(dir));
	};

	int FileStream::open(const char *path, unsigned int mode, const char **err) {
		if(*err != nullptr)
			return -1;

		if(i_fs.is_open()) {
			*err = "File stream is already open";
			return -1;
		}
		
		i_fs.open(path, i_fs_openmode_to_std(mode));
		if(i_fs.fail()) {
			*err = std::strerror(errno);
			return -1;
		}

		seekg(0, beg);
		seekp(0, beg);

		return 0;
	};

	bool FileStream::is_open(void) {
		return i_fs.is_open();
	};
	
	void FileStream::close(void) {
		if(is_open())
			i_fs.close();
	};

	int FileStream::read(u8 *s, u64 n, const char **err) {
		if(*err != nullptr)
			return -1;
		
		(void)i_fs.read((char *)s, n);

		if(i_fs.fail()) {
			*err = std::strerror(errno);
			return -1;
		}
			
		return 0;
	};

	int FileStream::write(u8 *s, u64 n, const char **err) {
		if(*err != nullptr)
			return -1;
		
		(void)i_fs.write((char *)s, n);

		if(i_fs.fail()) {
			*err = std::strerror(errno);
			return -1;
		}
		
		return 0;
	};
};

#endif
