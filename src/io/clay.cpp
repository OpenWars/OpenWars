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
#ifndef __openwars__io__clay__cpp__
#define __openwars__io__clay__cpp__

#include "../nuclei.hpp"
#include "../misc/auditor.hpp"
#include "mouse.hpp"
#include "log.hpp"
#include <raymath.h>
#include <unordered_map>
#include <string>
#include <cmath>

#include "clay_math.hpp"
#include "../visual/visual.hpp"

namespace Raylib {
	#include <raylib.h>
};

extern "C" {
	#define CLAY_IMPLEMENTATION
	#include "../../clay/clay.h"
};

namespace OpenWars {
	namespace Clay {
		bool i_is_init = false;
		u64 i_audit_id = 0;
		u8 *i_min_data_ptr = nullptr;

		std::unordered_map<u32, font_t> i_fonts;

		Raylib::Matrix i_r_matrix_i2r(I_Matrix m) {
			return Raylib::Matrix {
				m.m0, m.m4, m.m8, m.m12,
				m.m1, m.m5, m.m9, m.m13,
				m.m2, m.m6, m.m10, m.m14,
				m.m3, m.m7, m.m11, m.m15,
			};
		};

		I_Matrix i_r_matrix_r2i(Raylib::Matrix m) {
			return I_Matrix {
				m.m0, m.m4, m.m8, m.m12,
				m.m1, m.m5, m.m9, m.m13,
				m.m2, m.m6, m.m10, m.m14,
				m.m3, m.m7, m.m11, m.m15,
			};
		};

		Raylib::Vector3 i_r_vector3_i2r(I_Vector3 v) {
			return Raylib::Vector3 { v.x, v.y, v.z };
		};

		I_Vector3 i_r_vector3_r2i(Raylib::Vector3 v) {
			return I_Vector3 { v.x, v.y, v.z };
		};

		Raylib::Color clay_color_to_raylib_color(Clay_Color color) {
			return {
				(u8)std::roundf(color.r),
				(u8)std::roundf(color.g),
				(u8)std::roundf(color.b),
				(u8)std::roundf(color.a),
			};
		};

		typedef enum {
			CUSTOM_LAYOUT_ELEMENT_TYPE_3D_MODEL
		} CustomLayoutElementType;

		typedef struct {
			Raylib::Model model;
			float scale;
			Raylib::Vector3 position;
			Raylib::Matrix rotation;
		} CustomLayoutElement_3DModel;

		typedef struct {
			CustomLayoutElementType type;
			union {
				CustomLayoutElement_3DModel model;
			};
		} CustomLayoutElement;

		Raylib::Ray GetScreenToWorldPointWithZDistance(
			Raylib::Vector2 pos,
			int screen_width,
			int screen_height,
			float z_dist,
			Raylib::Camera *r_cam
		) {
			Raylib::Ray ray = {
				{ 0.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 0.0f },
			};

			// Calculate normalized device coordinates.
			// NOTE: y value is negative.
			Raylib::Vector3 device_coords = {
				(2.0f * pos.x) / (float)screen_width - 1.0f,
				1.0f - (2.0f * pos.y) / (float)screen_height,
				1.0f,
			};

			// Calculate view matrix from camera look at.
			Raylib::Matrix mat_view = i_r_matrix_i2r(matrix_look_at(i_r_vector3_r2i(r_cam->position), i_r_vector3_r2i(r_cam->target), i_r_vector3_r2i(r_cam->up)));

			Raylib::Matrix mat_proj = i_r_matrix_i2r(matrix_identity());

			if(r_cam->projection == Raylib::CAMERA_PERSPECTIVE)
				mat_proj = i_r_matrix_i2r(matrix_perspective(
					r_cam->fovy * DEG2RAD,
					((double)screen_width / (double)screen_height),
					0.01,
					z_dist
				));
			
			if(r_cam->projection == Raylib::CAMERA_ORTHOGRAPHIC) {
				double top = r_cam->fovy / 2.0;
				double right = top * ((double)screen_width / (double)screen_height);

				mat_proj = i_r_matrix_i2r(matrix_ortho(-right, right, -top, top, 0.01, 1000.0));
			}

			Raylib::Vector3 near_point = i_r_vector3_i2r(vector3_unproject(
				i_r_vector3_r2i(Raylib::Vector3 {
					device_coords.x,
					device_coords.y,
					0.0f,
				}),
				i_r_matrix_r2i(mat_proj),
				i_r_matrix_r2i(mat_view)
			));

			Raylib::Vector3 far_point = i_r_vector3_i2r(vector3_unproject(
				i_r_vector3_r2i(Raylib::Vector3 {
					device_coords.x,
					device_coords.y,
					1.0f,
				}),
				i_r_matrix_r2i(mat_proj),
				i_r_matrix_r2i(mat_view)
			));

			Raylib::Vector3 dir = i_r_vector3_i2r(vector3_normalize(vector3_subtract(i_r_vector3_r2i(far_point), i_r_vector3_r2i(near_point))));

			ray.position = far_point;
			ray.direction = dir;

			return ray;
		};

		extern "C" Clay_Dimensions i_openwars_clay_measure_text(
			Clay_String *text, Clay_TextElementConfig *config
		) {
			float text_height = config->fontSize;

			if(i_fonts.find(config->fontId) == i_fonts.end()) {
				log_error("[Clay] MeasureText(): Font %i was not found.\n", config->fontId);
				return { 0, 0 };
			}

			const char *err = nullptr;

			font_t font = i_fonts[config->fontId];
			void *p_font = get_font_handle(&font, &err);

			if(p_font == nullptr) {
				log_error("[Clay] MeasureText(): Font %i's handler was not found: %s\n", err);
				return { 0, 0 };
			}

			Raylib::Font r_font = *(Raylib::Font *)p_font;

			float scale = (float)config->fontSize / (float)r_font.baseSize;
			float max_text_width = 0.0f;
			float line_text_width = 0.0f;

			for(int i = 0; i < text->length; i++) {
				if(text->chars[i] == '\n') {
					max_text_width = std::fmax(max_text_width, line_text_width);
					line_text_width = 0;
					continue;
				}

				int index = text->chars[i] - 32;

				if(r_font.glyphs[index].advanceX != 0)
					line_text_width += r_font.glyphs[index].advanceX;
				else
					line_text_width += (r_font.recs[index].width + r_font.glyphs[index].offsetX);
			};

			max_text_width = std::fmax(max_text_width, line_text_width);

			return {
				max_text_width * scale,
				text_height,
			};
		};

		int init(const char **err) {
			if(*err != nullptr)
				return -1;

			if(i_is_init != false) {
				*err = "Clay is already initialized";
				return -1;
			}

			i_fonts.clear();

			if(i_min_data_ptr != nullptr) {
				vfree(i_min_data_ptr);
				i_min_data_ptr = nullptr;
			}

			u32 min_mem_size = Clay_MinMemorySize();
			i_min_data_ptr = valloc<u8>(min_mem_size, err);

			if(i_min_data_ptr == nullptr)
				return -1;

			i_audit_id = audit(AUDITOR_RESOURCES::MISC, "Clay (GUI)", i_min_data_ptr, err);
			if(*err != nullptr) {
				vfree(i_min_data_ptr);
				return -1;
			}

			Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(min_mem_size, i_min_data_ptr);

			Clay_Initialize(arena, Clay_Dimensions {
				(float)get_window_width(),
				(float)get_window_height(),
			});

			Clay_SetPointerState({
				get_mouse_position_x(),
				get_mouse_position_y(),
			}, is_mouse_down());

			Clay_UpdateScrollContainers(true, {
				get_mouse_wheel_x(),
				get_mouse_wheel_y(),
			}, get_delta_time());

			i_is_init = true;
			return 0;
		};

		void deinit(void) {
			i_is_init = false;
			i_fonts.clear();

			if(i_min_data_ptr != nullptr) {
				vfree(i_min_data_ptr);
				i_min_data_ptr = nullptr;
			}

			const char *err = nullptr;

			(void)deaudit(i_audit_id, &err);
			(void)err;
		};

		void update(void) {
			Clay_SetLayoutDimensions({
				(float)get_window_width(),
				(float)get_window_height(),
			});

			Clay_SetPointerState({
				get_mouse_position_x(),
				get_mouse_position_y(),
			}, is_mouse_down());

			Clay_UpdateScrollContainers(true, {
				get_mouse_wheel_x(),
				get_mouse_wheel_y(),
			}, get_delta_time());
		};

		void start_frame(void) {
			Clay_BeginLayout();
		};

		void end_frame(void *cam) {
			Clay_RenderCommandArray render_cmds_arr = Clay_EndLayout();

			for(u32 i = 0; i < render_cmds_arr.length; i++) {
				Clay_RenderCommand *render_cmd = Clay_RenderCommandArray_Get(&render_cmds_arr, i);

				Clay_BoundingBox bounding_box = render_cmd->boundingBox;

				switch(render_cmd->commandType) {
					case CLAY_RENDER_COMMAND_TYPE_TEXT: {
						Clay_TextElementConfig *config = render_cmd->config.textElementConfig;
						
						std::string r_str;

						for(int i = 0; i < render_cmd->text.length; i++)
							r_str.push_back(render_cmd->text.chars[i]);

						if(i_fonts.find(config->fontId) == i_fonts.end()) {
							log_error("[Clay] EndFrame(): Font %i was not found.\n", config->fontId);
							return;
						}

						const char *err = nullptr;

						font_t font = i_fonts[config->fontId];
						void *p_font = get_font_handle(&font, &err);

						if(p_font == nullptr) {
							log_error("[Clay] EndFrame(): Font %i's handler was not found: %s\n", config->fontId, err);
							return;
						}

						Raylib::Font r_font = *(Raylib::Font *)p_font;

						Raylib::DrawTextEx(
							r_font, r_str.c_str(),
							Raylib::Vector2 {
								bounding_box.x,
								bounding_box.y,
							},
							(float)config->fontSize,
							(float)config->letterSpacing,
							clay_color_to_raylib_color(config->textColor)
						);

						break;
					};
					
					case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
						Raylib::Texture2D r_tex = *(Raylib::Texture2D *)render_cmd->config.imageElementConfig->imageData;

						Raylib::DrawTextureEx(
							r_tex,
							Raylib::Vector2 {
								bounding_box.x,
								bounding_box.y,
							},
							0.0f,
							bounding_box.width / (float)r_tex.width,
							Raylib::WHITE
						);

						break;
					};

					case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
						Raylib::BeginScissorMode(
							(int)std::roundf(bounding_box.x),
							(int)std::roundf(bounding_box.y),
							(int)std::roundf(bounding_box.width),
							(int)std::roundf(bounding_box.height)
						);

						break;
					};
					
					case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
						Raylib::EndScissorMode();
					};

					case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
						Clay_RectangleElementConfig *config = render_cmd->config.rectangleElementConfig;

						if(config->cornerRadius.topLeft > 0.0f) {
							float radius = config->cornerRadius.topLeft * 2.0f;

							if(bounding_box.width > bounding_box.height)
								radius /= bounding_box.height;
							else
								radius /= bounding_box.width;

							Raylib::DrawRectangleRounded(
								Raylib::Rectangle {
									bounding_box.x,
									bounding_box.y,
									bounding_box.width,
									bounding_box.height,
								},
								radius,
								8,
								clay_color_to_raylib_color(config->color)
							);

							break;
						}

						Raylib::DrawRectangle(
							bounding_box.x,
							bounding_box.y,
							bounding_box.width,
							bounding_box.height,
							clay_color_to_raylib_color(config->color)
						);

						break;
					};

					case CLAY_RENDER_COMMAND_TYPE_BORDER: {
						Clay_BorderElementConfig *config = render_cmd->config.borderElementConfig;

						if(config->left.width > 0.0f) {
							Raylib::DrawRectangle(
								(int)std::roundf(bounding_box.x),
								(int)std::roundf(bounding_box.y + config->cornerRadius.topLeft),
								(int)config->left.width,
								(int)std::roundf(bounding_box.height - config->cornerRadius.topLeft - config->cornerRadius.bottomLeft),
								clay_color_to_raylib_color(config->left.color)
							);
						}

						if(config->right.width > 0) {
							Raylib::DrawRectangle(
								(int)std::roundf(bounding_box.x + bounding_box.width - config->right.width),
								(int)std::roundf(bounding_box.y + config->cornerRadius.topRight),
								(int)config->right.width,
								(int)std::roundf(bounding_box.height - config->cornerRadius.topRight - config->cornerRadius.bottomRight),
								clay_color_to_raylib_color(config->right.color)
							);
						}

						if(config->top.width > 0) {
							Raylib::DrawRectangle(
								(int)std::roundf(bounding_box.x + config->cornerRadius.topLeft),
								(int)std::roundf(bounding_box.y),
								(int)std::roundf(bounding_box.width - config->cornerRadius.topLeft - config->cornerRadius.topRight),
								(int)config->top.width,
								clay_color_to_raylib_color(config->top.color)
							);
						}

						if(config->bottom.width > 0) {
							Raylib::DrawRectangle(
								(int)std::roundf(bounding_box.x + config->cornerRadius.bottomLeft),
								(int)std::roundf(bounding_box.y + bounding_box.height - config->bottom.width),
								(int)std::roundf(bounding_box.width - config->cornerRadius.bottomLeft - config->cornerRadius.bottomRight),
								(int)config->bottom.width,
								clay_color_to_raylib_color(config->bottom.color)
							);
						}

						if(config->cornerRadius.topLeft > 0) {
							Raylib::DrawRing(
								Raylib::Vector2 {
										std::roundf(bounding_box.x + config->cornerRadius.topLeft),
										std::roundf(bounding_box.y + config->cornerRadius.topLeft),
									},
									std::roundf(config->cornerRadius.topLeft - config->top.width),
									config->cornerRadius.topLeft, 180, 270,
									10, clay_color_to_raylib_color(config->top.color)
								);
						}

						if(config->cornerRadius.topRight > 0) {
							Raylib::DrawRing(
								Raylib::Vector2 {
									std::roundf(bounding_box.x + bounding_box.width - config->cornerRadius.topRight),
									std::roundf(bounding_box.y + config->cornerRadius.topRight),
								},
								std::roundf(config->cornerRadius.topRight - config->top.width),
								config->cornerRadius.topRight,
								270,
								360,
								10,
								clay_color_to_raylib_color(config->top.color)
							);
						}

						if(config->cornerRadius.bottomLeft > 0) {
							Raylib::DrawRing(
								Raylib::Vector2 {
									std::roundf(bounding_box.x + config->cornerRadius.bottomLeft),
									std::roundf(bounding_box.y + bounding_box.height - config->cornerRadius.bottomLeft),
								},
								std::roundf(config->cornerRadius.bottomLeft - config->top.width),
								config->cornerRadius.bottomLeft,
								90,
								180,
								10,
								clay_color_to_raylib_color(config->bottom.color)
							);
						}

						if(config->cornerRadius.bottomRight > 0) {
							Raylib::DrawRing(
								Raylib::Vector2 {
									std::roundf(bounding_box.x + bounding_box.width - config->cornerRadius.bottomRight),
									std::roundf(bounding_box.y + bounding_box.height - config->cornerRadius.bottomRight),
								},
								std::roundf(config->cornerRadius.bottomRight - config->bottom.width),
								config->cornerRadius.bottomRight,
								0.1,
								90,
								10,
								clay_color_to_raylib_color(config->bottom.color)
							);
						}

						break;
					};

					case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
						CustomLayoutElement *custom_el = (CustomLayoutElement *)render_cmd->config.customElementConfig->customData;

						if(!custom_el)
							continue;

						switch((long int)custom_el->type) {
							case CUSTOM_LAYOUT_ELEMENT_TYPE_3D_MODEL: {
								Clay_BoundingBox root_box = render_cmds_arr.internalArray[0].boundingBox;

								float scale = CLAY__MIN(
									CLAY__MIN(1.0f, 768.0f / root_box.height) *
									CLAY__MAX(1.0f, root_box.width / 1024.0f),
									1.5f
								);

								Raylib::Ray ray_pos = GetScreenToWorldPointWithZDistance(
									Raylib::Vector2 {
										render_cmd->boundingBox.x +
											render_cmd->boundingBox.width / 2,
										render_cmd->boundingBox.y +
											(render_cmd->boundingBox.height / 2) +
											20,
									},
									(int)std::roundf(root_box.width),
									(int)std::roundf(root_box.height),
									140,
									(Raylib::Camera *)cam
								);

								Raylib::BeginMode3D(*(Raylib::Camera *)cam);

								Raylib::DrawModel(
									custom_el->model.model,
									ray_pos.position,
									custom_el->model.scale * scale,
									Raylib::WHITE
								);

								Raylib::EndMode3D();

								break;
							};

							default: {
								log_error("[Clay] EndFrame(): Custom element layout type %li was not found.\n", custom_el->type);
								
								break;
							};
						};

						break;
					};
					
					default: {
						log_error("[Clay] Unhandled render command of type %x.\n", (u32)render_cmd->commandType);
						break;
					};
				};
			};
		};
	};
};

#endif
