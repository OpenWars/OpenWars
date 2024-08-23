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
#ifndef __openwars__io__files__hpp__
#define __openwars__io__files__hpp__

#include "stream.hpp"
#include <fstream>

namespace OpenWars {
	i8 create_directories(const char *path);

	class FileStream : public BaseStream {
		public:
			typedef enum _flags {
				in = 1 << 0,
				out = 1 << 1,

				beg,
				cur,
				end,
			} flags;

		private:
			std::fstream fs;

			std::ios::openmode __flags_to_std_openmode(u16 mode);
			std::ios::seekdir __flags_to_std_seekdir(u16 mode);

		public:
			i8 open(const char *path, u16 mode, const char *err);
			bool is_open(void);
			void close(void);

			u64 tellg(void);
			void seekg(u64 pos);
			void seekg(i64 off, seekdir dir);
			
			u64 tellp(void);
			void seekp(u64 pos);
			void seekp(i64 off, seekdir dir);
			
			i8 read(u8 *s, u64 n, const char *err);
			i8 write(u8 *s, u64 n, const char *err);
	};
};

#endif
