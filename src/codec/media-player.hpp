/*
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

#ifndef __openwars__codec__media_player__hpp__
#define __openwars__codec__media_player__hpp__

#include "../defs.hpp"

namespace OpenWars {
	class MediaPlayer {
		private:
			char err_buff[128];
			uintptr_t i_ptr;

		public:
			typedef f32 time_t;

			typedef struct {
				u32	width;
				u32	height;

				// Big-Endian RGBA8888.
				u8	*data;
			} video_frame_t;

			typedef struct {
				u64		samples;
				// (-1.0)...(1.0) samples.
				float	*data;
			} audio_samples_t;

			ErrorOr<void> open(const char *filepath);
			void close(void);

			time_t time(void);
			void time(time_t t);

			void pause(void);
			void resume(void);

			ErrorOr<void> process(void);

			ErrorOr<video_frame_t *> get_frame(void);
			ErrorOr<audio_samples_t *> get_samples(void);
	};
};

#endif
