#ifndef __openwars__game__visual__visual__cpp__
#define __openwars__game__visual__visual__cpp__

#include "visual.hpp"
#include <new>

namespace Raylib {
	#include <raylib.h>
};

namespace OpenWars {
	ErrorOr<void> init_video(void) {
		// For now, it does nothing (Raylib inits video at InitWindow).
		return Error { nullptr };
	};

	typedef struct {
		bool				is_bitmap;
		Raylib::Texture2D	*tex;
	} c_texture_thingy_t;

	// "pixels" as Big-Endian RGBA8888, from top-left to bottom-right.
	ErrorOr<texture_t *> create_bitmap_texture(u16 width, u16 height, u8 *pixels) {
		texture_t *tex;
		c_texture_thingy_t *thing;
		Raylib::Texture2D *r_tex_ptr;

		try {
			tex = new texture_t;
			thing = new c_texture_thingy_t;
			r_tex_ptr = new Raylib::Texture2D;
		} catch(std::bad_alloc &e) {
			return Error {
				"Couldn't allocate enough memory for a bitmap texture"
			};
		};

		Raylib::Image r_img = Raylib::GenImageColor(width, height, Raylib::BLACK);

		Raylib::Color *r_col = Raylib::LoadImageColors(r_img);

		u64 iwh = (width * height * 4);

		for(u64 i = 0; i < iwh; i += 4) {
			r_col[i >> 2] = {
				pixels[i | 0x0],
				pixels[i | 0x1],
				pixels[i | 0x2],
				pixels[i | 0x3],
			};
		};

		*r_tex_ptr = Raylib::LoadTextureFromImage(r_img);

		Raylib::UnloadImageColors(r_col);
		Raylib::UnloadImage(r_img);

		tex->width = width;
		tex->height = height;
		thing->is_bitmap = true;
		thing->tex = r_tex_ptr;

		tex->data_ptr = (uintptr_t)thing;

		return tex;
	};

	ErrorOr<void> draw_texture(texture_t *texture, float x, float y, float w, float h, float a, float t) {
		(void)t;

		if(texture == nullptr)
			return Error { "Texture pointer is NULL" };

		if(texture->data_ptr == 0)
			return Error { "Texture data pointer is NULL" };

		c_texture_thingy_t *thing = (c_texture_thingy_t *)texture->data_ptr;

		if(thing->is_bitmap) {
			Raylib::DrawTexturePro(
				*thing->tex,
				{
					0, 0,
					(float)thing->tex->width,
					(float)thing->tex->height,
				},
				{
					x, y,
					w, h,
				},
				{ 0.0, 0.0 },
				a,
				Raylib::WHITE
			);

			return Error { nullptr };
		} else {
			return Error { "Vector textures are TODOs" };
		}
	};

	void free_texture(texture_t *texture) {
		if(texture == nullptr)
			return;
		if(texture->data_ptr == 0)
			return;

		c_texture_thingy_t *thing = (c_texture_thingy_t *)texture->data_ptr;

		if(thing->is_bitmap) {
			if(thing->tex != nullptr) {
				Raylib::UnloadTexture(*thing->tex);
				thing->tex = nullptr;
			}

			delete thing;
			thing = nullptr;
		} else {
			// [TODO] Handle vector stuff.
			return;
		}

		delete texture;
		texture = nullptr;
	};

	ErrorOr<texture_t *> load_texture_from_file(const char *filepath) {
		texture_t *tex;
		c_texture_thingy_t *thing;

		try {
			tex = new texture_t;
			thing = new c_texture_thingy_t;
		} catch(std::bad_alloc &e) {
			return Error {
				"Couldn't allocate enough memory for a bitmap texture"
			};
		};

		Raylib::Image r_img = Raylib::LoadImage(filepath);
		Raylib::Texture2D r_tex = Raylib::LoadTextureFromImage(r_img);

		tex->width = r_img.width;
		tex->height = r_img.height;

		Raylib::UnloadImage(r_img);

		// [TODO] Handle vector-based images.
		thing->is_bitmap = true;
		thing->tex = (&r_tex);

		tex->data_ptr = (uintptr_t)thing;

		return tex;
	};

	void free_font(font_t *font) {
		if(font->data_ptr != (uintptr_t)nullptr) {
			Raylib::UnloadFont(*(Raylib::Font *)font->data_ptr);
			font->data_ptr = (uintptr_t)nullptr;
		}

		delete font;
	};

	ErrorOr<font_t *> load_font_from_file(const char *filepath) {
		font_t *font;
		Raylib::Font *r_font_ptr;

		try {
			font = new font_t;
			r_font_ptr = new Raylib::Font;
		} catch(std::bad_alloc &e) {
			return Error {
				"Couldn't allocate enough memory for a font"
			};
		};

		*r_font_ptr = Raylib::LoadFont(filepath);
		font->data_ptr = (uintptr_t)r_font_ptr;

		return font;
	};

	ErrorOr<void> draw_font(font_t *font, const char *text, float x, float y, float size, float spacing, color_t color) {
		if(font == nullptr)
			return Error { "Font is NULL" };
		if(font->data_ptr == (uintptr_t)nullptr)
			return Error { "Font pointer is NULL" };

		Raylib::DrawTextEx(	*(Raylib::Font *)font->data_ptr,
							text,
							{ x, y },
							size, spacing,
							Raylib::Color {
								color.r,
								color.g,
								color.b,
								color.a
							});
		
		return Error { nullptr };
	};
};

#endif
