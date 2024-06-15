#ifndef __openwars__io__files__hpp__
#define __openwars__io__files__hpp__

#include "stream.hpp"
#include <fstream>

namespace OpenWars {
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
			ErrorOr<void> open(const char *path, u16 mode);
			bool is_open(void);
			void close(void);

			u64 tellg(void);
			void seekg(u64 pos);
			void seekg(i64 off, seekdir dir);
			
			u64 tellp(void);
			void seekp(u64 pos);
			void seekp(i64 off, seekdir dir);
			
			ErrorOr<void> read(u8 *s, u64 n);
			ErrorOr<void> write(u8 *s, u64 n);
	};
};

#endif
