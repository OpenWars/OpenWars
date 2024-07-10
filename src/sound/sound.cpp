#ifndef __openwars__game__sound__sound__cpp__
#define __openwars__game__sound__sound__cpp__

#include "sound.hpp"
#include <new>

namespace Raylib {
	#include <raylib.h>
};

namespace OpenWars {
	ErrorOr<void> init_audio(void) {
		// For now, it does nothing (Raylib inits audio at InitWindow).
		return Error { nullptr };
	};

	typedef struct _c_isptr_t {
		Raylib::Music	r_music;
	} _isptr_t;

	ErrorOr<sound_t *> load_sound_from_file(const char *filepath) {
		_isptr_t *ip;
		sound_t *s;

		try {
			ip = new _isptr_t;
			s = new sound_t;
		} catch(std::bad_alloc &e) {
			return Error {
				"Couldn't allocate enough memory for a sound stream"
			};
		};

		// [TODO]
		ip->r_music = Raylib::LoadMusicStream(filepath);
		*s = {
			0,
			nullptr,
			-1.0,
			-1.0,
			(uintptr_t)ip,
		};

		return s;
	};
};

#endif
