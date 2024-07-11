#ifndef __openwars__game__visual__visual__cpp__
#define __openwars__game__visual__visual__cpp__

#include "visual.hpp"
#include "../misc/auditor.hpp"
#include <new>

namespace Raylib {
	#include <raylib.h>
};

namespace OpenWars {
	Raylib::Camera2D p_r_camera;

	ErrorOr<void> init_video(int width, int height, const char *title) {
		Raylib::InitWindow(width, height, title);

		while(Raylib::IsWindowReady() == false);

		p_r_camera = {
			{ 0.0, 0.0 },
			{ 0.0, 0.0 },
			0.0,
			1.0
		};

		Raylib::SetTargetFPS(60);

		return Error { nullptr };
	};

	ErrorOr<void> deinit_video(void) {
		Raylib::CloseWindow();

		return Error { nullptr };
	};

	ErrorOr<void> init_frame(void) {
		Raylib::BeginDrawing();
		Raylib::ClearBackground(Raylib::BLACK);

		Raylib::BeginMode2D(p_r_camera);

		return Error { nullptr };
	};

	ErrorOr<void> swap_buffers(void) {
		Raylib::EndMode2D();
		Raylib::EndDrawing();

		return Error { nullptr };
	};

	bool should_close_window(void) {
		return Raylib::WindowShouldClose();
	};

	typedef struct {
		Raylib::Texture2D	r_tex;
		u8					*pixels;
	} p_tex_t;

	ErrorOr<texture_t *> create_bitmap_texture(u32 width, u32 height) {
		texture_t *tex;
		p_tex_t *thing;

		try {
			tex = new texture_t;
			thing = new p_tex_t;
		} catch(std::bad_alloc &e) {
			return Error {
				"Couldn't allocate enough memory for a bitmap texture"
			};
		};

		Raylib::Image r_img = Raylib::GenImageColor(width, height, Raylib::BLACK);
		thing->r_tex = Raylib::LoadTextureFromImage(r_img);
		thing->pixels = nullptr;

		Raylib::UnloadImage(r_img);

		tex->width = width;
		tex->height = height;
		tex->p_data = (uintptr_t)thing;

		return tex;
	};

	// "pixels" as Big-Endian RGBA8888, from top-left to bottom-right.
	ErrorOr<void> update_bitmap_texture(texture_t *tex, u8 *pixels) {
		if(tex == nullptr)
			return Error { "Texture pointer is NULL" };
		if(tex->p_data == (uintptr_t)nullptr)
			return Error { "Texture data pointer is NULL" };

		p_tex_t *thing = (p_tex_t *)(tex->p_data);

		if(pixels == nullptr)
			pixels = thing->pixels;
		if(pixels == nullptr)
			return Error { "Texture pixel data pointer is NULL" };

		Raylib::UpdateTexture(thing->r_tex, pixels);

		return Error { nullptr };
	};

	ErrorOr<void> draw_texture(texture_t *texture, float x, float y, float w, float h, float a, float t) {
		(void)t;

		if(texture == nullptr)
			return Error { "Texture pointer is NULL" };

		if(texture->p_data == 0)
			return Error { "Texture data pointer is NULL" };

		p_tex_t *thing = (p_tex_t *)(texture->p_data);

		Raylib::DrawTexturePro(
			thing->r_tex,
			{
				0, 0,
				(float)(thing->r_tex.width),
				(float)(thing->r_tex.height),
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
	};

	void free_texture(texture_t *texture) {
		if(texture == nullptr)
			return;
		if(texture->p_data == (uintptr_t)nullptr)
			return;

		p_tex_t *thing = (p_tex_t *)(texture->p_data);

		Raylib::UnloadTexture(thing->r_tex);

		delete thing;
		thing = nullptr;

		delete texture;
		texture = nullptr;
	};

	ErrorOr<texture_t *> load_texture_from_file(const char *filepath) {
		texture_t *tex;
		p_tex_t *thing;

		try {
			tex = new texture_t;
			thing = new p_tex_t;
		} catch(std::bad_alloc &e) {
			return Error {
				"Couldn't allocate enough memory for a bitmap texture"
			};
		};

		Raylib::Image r_img = Raylib::LoadImage(filepath);
		thing->r_tex = Raylib::LoadTextureFromImage(r_img);

		tex->width = r_img.width;
		tex->height = r_img.height;

		tex->p_data = (uintptr_t)thing;

		return tex;
	};

	void free_font(font_t *font) {
		if(font == nullptr)
			return;

		if(font->p_data != (uintptr_t)nullptr) {
			Raylib::UnloadFont(*(Raylib::Font *)font->p_data);
			font->p_data = (uintptr_t)nullptr;
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
		font->p_data = (uintptr_t)r_font_ptr;

		return font;
	};

	ErrorOr<void> draw_font(font_t *font, const char *text, float x, float y, float size, float spacing, color_t color) {
		if(font == nullptr)
			return Error { "Font is NULL" };
		if(font->p_data == (uintptr_t)nullptr)
			return Error { "Font pointer is NULL" };

		Raylib::DrawTextEx(	*(Raylib::Font *)font->p_data,
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
