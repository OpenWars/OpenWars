#ifndef __openwars__codec__mpeg1__dec__hpp__
#define __openwars__codec__mpeg1__dec__hpp__

#include "../player.hpp"
#include "../../io/files.hpp"

namespace OpenWars {
	namespace Codec {
		class DecMPEG1 : public VideoPlayer {
			private:
				const char *err_str = nullptr;
				
				FileStream fs;

				typedef enum _STAGE {
					NOT_OPEN,
					HEADER_NOT_LOADED,
				} STAGE;

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
