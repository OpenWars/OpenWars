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
