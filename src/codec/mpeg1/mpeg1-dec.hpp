#ifndef __openwars__codec__mpeg1__dec__hpp__
#define __openwars__codec__mpeg1__dec__hpp__

#include "../player.hpp"
#include "../../io/files.hpp"

namespace OpenWars {
	namespace Codec {
		class DecMPEG1 : public VideoPlayer {
			private:
				typedef enum _STAGE {
					NOT_OPEN,
					HEADER_NOT_LOADED,
				} STAGE;

				typedef enum _PACKETS {
					START_CODE_PREFIX = 0x000001,
					PROGRAM_END = 0xb9,
					PACK_HEADER = 0xba,
					MPEG_AUDIO = 0xc0,
					MPEG_VIDEO = 0xe0,
					MPEG_AUDIO_MASK = 0x1f,
					MPEG_VIDEO_MASK = 0x0f,
				} PACKETS;

				const char *err_str = nullptr;
				
				FileStream fs;
				STAGE stage = STAGE::NOT_OPEN;

				int load_header(void);

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
	};
};

#endif
