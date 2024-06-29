#ifndef __openwars__game__visual__visual__hpp__
#define __openwars__game__visual__visual__hpp__

#include "../defs.hpp"

namespace OpenWars {
	typedef struct {
		u16				width;
		u16				height;
		uintptr_t		data_ptr;
	} texture_t;

	// "pixels" as Big-Endian RGBA8888, from top-left to bottom-right.
	ErrorOr<texture_t *> create_bitmap_texture(u16 width, u16 height, u8 *pixels);

	ErrorOr<void> draw_texture(texture_t *texture, float x, float y, float w, float h, float a, float t);

	void free_texture(texture_t *texture);
};

#endif
