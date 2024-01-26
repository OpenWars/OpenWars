#ifndef __openwars__game__map__crc32__cxx__
#define __openwars__game__map__crc32__cxx__

#include <cstdint>

// Credits:
// * https://gist.github.com/timepp/1f678e200d9e0f2a043a9ec6b3690635

namespace CRC {
	bool crc32_initialized = false;
	uint32_t *crc32_table;
	
	void init_crc32(void) {
		if(crc32_initialized) return;
		
		crc32_table = new uint8_t[256];
		uint32_t polynomial = 0xedb88320, j, k;
		
		for(uint32_t i = 0; i < 256; i++) {
			j = i;
			
			for(k = 0; k < 8; k++) {
				j >>= 1;
				
				if(j & 0x1) j ^= polynomial;
			};
			
			crc32_table[i] = j;
		};
		
		crc32_initialized = true;
	};
	
	class CRC32 {
		private:
			int status_code;
			uint32_t hash;
		
		public:
			CRC32(void) {
				status_code = 0;
				hash = 0xffffffff;
				
				if(!crc32_initialized) {
					status_code = -1;
				}
			};
			
			void update(uint32_t length, uint8_t *buff, uint32_t offset) {
				if(!crc32_initialized) {
					status_code = -1;
					return;
				}
				
				uint8_t *ptr = buff + offset;
				
				for(uint32_t i = 0; i < length; i++) {
					hash = crc32_table[(hash ^ (*ptr++)) & 0xff] ^ (hash >> 8);
				};
			};
			
			uint32_t digest(void) {
				if(!crc32_initialized) {
					status_code = -1;
					return 0xffffffff;
				}
				
				return (hash ^ 0xffffffff);
			};
			
			int get_status_code(void) {
				return status_code;
			};
	};
};

#endif
