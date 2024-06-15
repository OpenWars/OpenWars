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
