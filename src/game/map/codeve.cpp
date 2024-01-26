#ifndef __openwars__game__map__codeve__cxx__
#define __openwars__game__map__codeve__cxx__

namespace Codeve1 {
	bool initialized = false;
	uint8_t *table;
	
	void init_codeve256(void) {
		table = new uint8_t[256];
		
		for(uint8_t i = 0;; i++) {
			table[i] = ((i ^ (i << 4)) ^ (i >> 2)) - i;
			if(i == 0xff) break;
		};
		
		initialized = true;
	};
	
	class Codeve256 {
		private:
			uint32_t iteration;
			uint8_t hash[32];
			int status_code;
			
			void small_update(uint8_t x) {
				for(uint8_t i = 0; i < 32; i++) {
					hash[i] =	(table[i ^ x] -
								(((uint8_t)(iteration & 0xff)) - table[i + x]));
				};
			};
		
		public:
			int get_status_code(void) {
				return status_code;
			};
			
			void update(uint32_t length, uint8_t *data, uint32_t offset) {
				if(!initialized) {
					status_code = -1;
					return;
				}
				
				uint8_t *ptr = data + offset;
				
				for(uint32_t i = 0; i < length; i++, iteration++) {
					small_update(*ptr++);
				};
				
				status_code = 0;
			};
			
			uint8_t *digest(void) {
				if(!initialized) {
					status_code = -1;
					return nullptr;
				}
				
				for(uint8_t i = 0; i < 32; i += 4) {
					hash[i | 0x0] ^= (uint8_t)((iteration >> 24) & 0xff) ^ 0xaa;
					hash[i | 0x1] ^= (uint8_t)((iteration >> 16) & 0xff) ^ 0xaa;
					hash[i | 0x2] ^= (uint8_t)((iteration >> 8) & 0xff) ^ 0xaa;
					hash[i | 0x3] ^= (uint8_t)(iteration & 0xff) ^ 0xaa;
				};
				
				status_code = 0;
				return hash;
			};
			
			void reset(void) {
				iteration = 0;
				
				for(uint8_t i = 0; i < 32; i++) {
					hash[i] = 0x00;
				};
				
				status_code = 0;
				if(!initialized) {
					status_code = -1;
				}
			};
			
			Codeve256(void) {
				reset();
			};
	};
};

#endif
