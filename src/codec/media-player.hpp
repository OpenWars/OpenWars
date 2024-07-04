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
