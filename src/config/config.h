#ifndef __openwars__config__config__h__
#define __openwars__config__config__h__

class Config {
	private:
		const char *err = nullptr;

	public:
		Config(void);

		const char *get_error(void);
};

#endif
