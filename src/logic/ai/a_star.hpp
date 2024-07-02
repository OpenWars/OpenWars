#ifndef __openwars__game__ai__a_star__hpp__
#define __openwars__game__ai__a_star__hpp__

#include "../../defs.hpp"
#include <vector>

namespace OpenWars {
	class AStar {
		public:
			// Returns:
			// -1 for error.
			// +1 for wall.
			// +2 for endng cell.
			typedef i8 (* get_tile_t)(u32 x, u32 y);

			typedef enum _step {
				top, left, right, down,
			} step;

			typedef struct {
				i32	x;
				i32 y;
			} vec2_t;
		
		private:
			u8	*cells = nullptr;

		public:
			//

			static std::vector<step> travel(vec2_t size, vec2_t pos, vec2_t goal, get_tile_t cb);
	};
};

#endif
