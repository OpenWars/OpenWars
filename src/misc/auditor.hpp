#ifndef __openwars__auditor__hpp__
#define __openwars__auditor__hpp__

#include "../defs.hpp"

namespace OpenWars {
	extern uintptr_t p_audits;

	typedef enum {
		TEXTURE, AUDIO_STREAM,
	} AUDITOR_RESOURCES;

	ErrorOr<void> init_auditor(void);
	void deinit_auditor(void);

	ErrorOr<void> audit(u32 res, const char *add);
	ErrorOr<void> deaudit(u32 res, const char *add);
};

#endif
