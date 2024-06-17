#ifndef __openwars__codec__video__hpp__
#define __openwars__codec__video__hpp__

#include "../defs.hpp"

namespace OpenWars {
	namespace Codec {
		typedef struct _video_frame_t {
			u32	width;
			u32	height;

			// Big-Endian RGBA8888.
			u8*	data;
			u64	data_length;
		} video_frame_t;

		typedef struct _audio_frame_t {
			// -1.0 to 1.0 samples.
			float*	samples;
			u64		samples_length;
		} audio_frame_t;

		typedef f32 time_t;

		class BasePlayer {
			public:
				const char *get_error(void);

				int open(const char *path);
				void close(void);

				time_t time(void);
				void time(time_t to);

				void pause(void);
				void resume(void);

				int process(void);

		};

		class VideoPlayer : public BasePlayer {
			public:
				// returns (NULL) if unsuccessful.
				video_frame_t *get_frame(void);
		};

		class AudioPlayer : public BasePlayer {
			public:
				u32 get_channels(void);
				
				// returns (NULL) if unsuccessful.
				audio_frame_t *get_frames(void);
		};
	};
};

#endif
