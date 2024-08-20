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

#include "../nuclei.hpp"

namespace OpenWars {
	struct color_t {
		u8	r = 0xff;
		u8	g = 0x00;
		u8	b = 0xff;
		u8	a = 0xff;
	};

	struct texture_t {
		u32		width;
		u32		height;
		void	*i_data;
	};

	struct font_t {
		void	*i_data = nullptr;
	};

	i8 init_video(int width, int height, const char *title, const char *err);
	void deinit_video(void);

	i8 init_frame(const char *err);
	i8 swap_buffers(const char *err);

	bool should_close_window(void);

	// [Texture]
	texture_t * create_bitmap_texture(u32 width, u32 height, const char *err);
	texture_t * load_texture_from_file(const char *filepath, const char *err);

	// "rgba" as Big-Endian RGBA8888, from left-to-right, top-to-bottom.
	i8 update_bitmap_texture(texture_t *tex, u8 *rgba, const char *err);

	void free_texture(texture_t *tex);

	i8 draw_texture(texture_t *tex, float x, float y, float w, float h, float a, float t, const char *err);

	// [Font]
	font_t * load_font_from_file(const char *filepath, const char *err);

	void free_font(font_t *font);

	i8 draw_font(font_t *font, const char *text, float x, float y, float size, float spacing, color_t color, const char *err);
};

#endif
