#ifndef __openwars__io__stream__hpp__
#define __openwars__io__stream__hpp__

#include "../defs.hpp"

namespace OpenWars {
	typedef struct { const char *error; } Error;
	
	template<typename Type>
	class ErrorOr {
		private:
			Type val;
		
		public:
			const char *error = nullptr;
			
			ErrorOr(void) {
				error = nullptr;
			};
			
			ErrorOr(Error error_struct) {
				error = error_struct.error;
			};
			
			ErrorOr(Type value_pointer) {
				error = nullptr;
				val = value_pointer;
			};
			
			Type value(void) { return val; };
	};
	
	template<typename Type>
	class ErrorOr<Type*> {
		private:
			Type *val = nullptr;
		
		public:
			const char *error = nullptr;
			
			ErrorOr(void) {
				error = nullptr;
			};
			
			ErrorOr(Error error_struct) {
				error = error_struct.error;
			};
			
			ErrorOr(Type *value_pointer) {
				error = nullptr;
				val = value_pointer;
			};
			
			Type *value(void) { return val; };
	};
	
	template<typename Type>
	class ErrorOr<Type&> {
		public:
			const char *error = nullptr;
			Type* value_pointer = nullptr;
			
			ErrorOr(void) {
				error = nullptr;
			};
			
			ErrorOr(Error error_struct) {
				error = error_struct.error;
			};
			
			ErrorOr(Type *val_ptr) {
				error = nullptr;
				value_pointer = val_ptr;
			};
			
			Type& value(void) { return (*value_pointer); };
	};
	
	template<>
	class ErrorOr<void> {
		public:
			const char *error = nullptr;
			
			ErrorOr(Error error_struct) { error = error_struct.error; };
	};

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
			ErrorOr<float> readF64(void);
			
			ErrorOr<void> writeF32(float value);
			ErrorOr<void> writeF64(float value);
	};
};

#endif
