#ifndef __openwars__game__visual__visual__hpp__
#define __openwars__game__visual__visual__hpp__

#include "../defs.hpp"

namespace OpenWars {
	typedef int (*texture_callback_t)(float x, float y, float w, float h, float t);

	typedef struct {
		u16					width;
		u16					height;
		// Big-Endian RGBA8888 format.
		u8					*data;
		texture_callback_t	callback;
	} texture_t;

	extern ErrorOr<texture_t *> create_bitmap_texture(u16 width, u16 height, u8 *pixels);
};

#endif
