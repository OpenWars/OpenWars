#ifndef __openwars__game__sound__sound__hpp__
#define __openwars__game__sound__sound__hpp__

#include "../defs.hpp"

namespace OpenWars {
	typedef struct {
		u64			samples;
		// (-1.0)...(1.0) samples.
		float		*data;
		float		time;
		float		duration;

		uintptr_t	data_ptr;
	} sound_t;

	ErrorOr<sound_t *> load_sound_from_file(const char *filepath);
	ErrorOr<void> update_sound(sound_t *s);
	void free_sound(sound_t *s);
};

#endif
