#ifndef __openwars__config__config__h__
#define __openwars__config__config__h__

namespace OpenWars {
	class Config {
		private:
			const char *err_str = nullptr;
			char *profile_path = nullptr;

			int create_dirs(const char *path);
			int get_profile_path(void);
			int create_blank_profile(void);

		public:
			Config(void);
			~Config(void);

			const char *get_error(void);

			int load(void);
	};
};

#endif
