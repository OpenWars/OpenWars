#ifndef __openwars__defs__hpp__
#define __openwars__defs__hpp__

#include <cstdint>

namespace OpenWars {
	typedef uint8_t u8;
	typedef uint16_t u16;
	typedef uint32_t u32;
	typedef uint64_t u64;
	
	typedef int8_t i8;
	typedef int16_t i16;
	typedef int32_t i32;
	typedef int64_t i64;
	
	typedef float f32;
	typedef double f64;

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
};

#endif
