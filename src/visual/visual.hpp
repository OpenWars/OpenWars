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
#ifndef __openwars__game__visual__visual__hpp__
#define __openwars__game__visual__visual__hpp__

#include "../defs.hpp"

namespace OpenWars {
	typedef struct _color_t {
		u8	r = 0xff;
		u8	g = 0x00;
		u8	b = 0xff;
		u8	a = 0xff;
	} color_t;

	typedef struct {
		u32				width;
		u32				height;
		uintptr_t		p_data;
	} texture_t;

	typedef struct {
		uintptr_t		p_data;
	} font_t;

	ErrorOr<void> init_video(int width, int height, const char *title);
	ErrorOr<void> deinit_video(void);

	ErrorOr<void> init_frame(void);
	ErrorOr<void> swap_buffers(void);

	bool should_close_window(void);

	// [Texture]
	ErrorOr<texture_t *> create_bitmap_texture(u32 width, u32 height);
	ErrorOr<texture_t *> load_texture_from_file(const char *filepath);

	// "pixels" as Big-Endian RGBA8888, from top-left to bottom-right.
	ErrorOr<void> update_bitmap_texture(texture_t *tex, u8 *pixels);

	void free_texture(texture_t *texture);

	ErrorOr<void> draw_texture(texture_t *texture, float x, float y, float w, float h, float a, float t);

	// [Font]
	ErrorOr<font_t *> load_font_from_file(const char *filepath);

	void free_font(font_t *font);

	ErrorOr<void> draw_font(font_t *font, const char *text, float x, float y, float size, float spacing, color_t color);
};

#endif
