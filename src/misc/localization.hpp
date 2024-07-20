#ifndef __openwars__misc__localization__hpp__
#define __openwars__misc__localization__hpp__

#include "../defs.hpp"
#include "../io/files.hpp"
#include <unordered_map>

namespace OpenWars {
	namespace L10N {
		typedef std::unordered_map<const char *, const char *> info_t;

		extern void *s_lang;

		ErrorOr<void> init_localization(void);
		void deinit_localization(void);

		ErrorOr<void> load_language(const char *s);
		const char *get_text(u32 id, info_t &info);
	};
};

#endif
