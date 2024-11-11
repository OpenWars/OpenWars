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
#ifndef __openwars__game__visual__visual__cpp__
#define __openwars__game__visual__visual__cpp__

#include "visual.hpp"
#include "../misc/auditor.hpp"

namespace Raylib {
	#include <raylib.h>
};

namespace OpenWars {
	struct i_tex_t {
		Raylib::Texture2D	r_tex;
		u8					*rgba = nullptr;
		u64					audit_id = (u64)(-1);
	};

	struct i_fnt_t {
		Raylib::Font	r_fnt;
		u64				audit_id = (u64)(-1);
	};

	Raylib::Camera3D i_rcamera;
	u64 i_video_audit_id = (u64)(-1);

	i8 init_video(int width, int height, const char *title, const char **err) {
		if(*err != nullptr)
			return -1;

		Raylib::SetExitKey(Raylib::KEY_NULL);
		Raylib::InitWindow(width, height, title);

		while(Raylib::IsWindowReady() == false);

		i_rcamera = {
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			0.0f,
			0
		};

		Raylib::SetTargetFPS(60);

		i_video_audit_id = audit(AUDITOR_RESOURCES::MISC, "init_video", err);
		if(*err != nullptr)
			return -1;

		return 0;
	};

	void deinit_video(void) {
		Raylib::CloseWindow();
		(void)deaudit(i_video_audit_id, nullptr);
	};

	i8 init_frame(const char **err) {
		Raylib::BeginDrawing();
		Raylib::ClearBackground(Raylib::BLACK);

		Raylib::BeginMode3D(i_rcamera);

		(void)err;
		return 0;
	};

	i8 swap_buffers(const char **err) {
		Raylib::EndMode3D();
		Raylib::EndDrawing();

		(void)err;
		return 0;
	};

	bool should_close_window(void) {
		return Raylib::WindowShouldClose();
	};

	// [Texture]
	texture_t * create_bitmap_texture(u32 width, u32 height, const char **err) {
		if(*err != nullptr)
			return nullptr;

		texture_t *tex = valloc<texture_t>(err);
		if(tex == nullptr)
			return nullptr;

		i_tex_t *itex = valloc<i_tex_t>(err);
		if(tex == nullptr) {
			vfree(tex);
			return nullptr;
		}

		itex->audit_id = audit(AUDITOR_RESOURCES::TEXTURE, "create_bitmap_texture", err);
		if(*err != nullptr) {
			vfree(tex);
			vfree(itex);

			return nullptr;
		}

		Raylib::Image rimg = Raylib::GenImageColor(width, height, Raylib::BLACK);

		itex->r_tex = Raylib::LoadTextureFromImage(rimg);
		itex->rgba = nullptr;

		Raylib::UnloadImage(rimg);

		tex->width = width;
		tex->height = height;
		tex->i_data = itex;

		return tex;
	};

	texture_t * load_texture_from_file(const char *filepath, const char **err) {
		if(*err != nullptr)
			return nullptr;

		if(filepath == nullptr)
			{ *err = "The 'filepath' parameter is NULL"; return nullptr; }

		texture_t *tex = valloc<texture_t>(err);
		if(tex == nullptr)
			return nullptr;

		i_tex_t *itex = valloc<i_tex_t>(err);
		if(tex == nullptr) {
			vfree(tex);
			return nullptr;
		}

		itex->audit_id = audit(AUDITOR_RESOURCES::TEXTURE, filepath, err);
		if(*err != nullptr) {
			vfree(tex);
			vfree(itex);

			return nullptr;
		}

		Raylib::Image rimg = Raylib::LoadImage(filepath);

		itex->r_tex = Raylib::LoadTextureFromImage(rimg);
		itex->rgba = nullptr;
		tex->width = rimg.width;
		tex->height = rimg.height;
		tex->i_data = itex;

		Raylib::UnloadImage(rimg);

		return tex;
	};

	// "rgba" as Big-Endian RGBA8888, from top-left to bottom-right.
	i8 update_bitmap_texture(texture_t *tex, u8 *rgba, const char **err) {
		if(*err != nullptr)
			return -1;
		if(tex == nullptr)
			{ *err = "The 'tex' parameter is NULL"; return -1; }
		if(tex->i_data == nullptr)
			{ *err = "The 'i_data' field is NULL"; return -1; }
		
		if(rgba == nullptr)
			{ *err = "The 'pixels' parameter is NULL"; return -1; }

		i_tex_t *itex = (i_tex_t *)(tex->i_data);

		Raylib::UpdateTexture(itex->r_tex, rgba);

		return 0;
	};

	void free_texture(texture_t *tex) {
		if(tex == nullptr)
			return;

		if(tex->i_data != nullptr) {
			i_tex_t *itex = (i_tex_t *)(tex->i_data);

			Raylib::UnloadTexture(itex->r_tex);
			
			if(itex->rgba != nullptr)
				vfree(itex->rgba);
			
			(void)deaudit(itex->audit_id, nullptr);

			vfree(itex);
		}

		vfree(tex);
	};

	i8 draw_texture(texture_t *tex, float x, float y, float w, float h, float a, float t, const char **err) {
		(void)t; // Unused for now.

		if(*err != nullptr)
			return -1;
		if(tex == nullptr)
			{ *err = "The 'tex' parameter is NULL"; return -1; }
		if(tex->i_data == nullptr)
			{ *err = "The 'i_data' field is NULL"; return -1; }
		
		i_tex_t *itex = (i_tex_t *)(tex->i_data);
		
		Raylib::DrawTexturePro(
			itex->r_tex,
			{
				0, 0,
				(float)(itex->r_tex.width),
				(float)(itex->r_tex.height),
			},
			{
				x, y,
				w, h,
			},
			{ 0.0, 0.0 },
			a,
			Raylib::WHITE
		);

		return 0;
	};

	// [Font]
	font_t * load_font_from_file(const char *filepath, const char **err) {
		if(*err != nullptr)
			return nullptr;

		if(filepath == nullptr)
			{ *err = "The 'filepath' parameter is NULL"; return nullptr; }

		font_t *fnt = valloc<font_t>(err);
		if(fnt == nullptr)
			return nullptr;

		i_fnt_t *ifnt = valloc<i_fnt_t>(err);
		if(ifnt == nullptr) {
			vfree(fnt);
			return nullptr;
		}

		ifnt->audit_id = audit(AUDITOR_RESOURCES::FONT, filepath, err);
		if(*err != nullptr) {
			vfree(fnt);
			vfree(ifnt);

			return nullptr;
		}

		ifnt->r_fnt = Raylib::LoadFont(filepath);
		fnt->i_data = ifnt;

		return fnt;
	};

	void free_font(font_t *font) {
		if(font == nullptr)
			return;

		if(font->i_data != nullptr) {
			i_fnt_t *ifnt = (i_fnt_t *)(font->i_data);

			Raylib::UnloadFont(ifnt->r_fnt);
			(void)deaudit(ifnt->audit_id, nullptr);
			vfree(ifnt);
		}

		vfree(font);
	};

	i8 draw_font(font_t *font, const char *text, float x, float y, float size, float spacing, color_t color, const char **err) {
		if(*err != nullptr)
			return -1;
		if(text == nullptr)
			{ *err = "The 'text' parameter is NULL"; return -1; }
		if(font == nullptr)
			{ *err = "The 'font' parameter is NULL"; return -1; }
		if(font->i_data == nullptr)
			{ *err = "The 'i_data' field is NULL"; return -1; }
		
		i_fnt_t *ifnt = (i_fnt_t *)(font->i_data);
		
		Raylib::DrawTextEx(	ifnt->r_fnt,
							text,
							{ x, y },
							size, spacing,
							Raylib::Color {
								color.r,
								color.g,
								color.b,
								color.a
							});
		
		return 0;
	};

	int get_window_width(void) {
		return Raylib::GetScreenWidth();
	};

	int get_window_height(void) {
		return Raylib::GetScreenHeight();
	};
};

#endif
