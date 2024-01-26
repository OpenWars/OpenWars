#ifndef __openwars__config__config__cxx__
#define __openwars__config__config__cxx__

#include "config.h"

#include <filesystem>
#include <cstdlib>
#include <cstring>

namespace Config {
	int initialize(void) {
		// We check if the env. variable "HOME" is set, if it's true, then we
		// set "CONFIG_PATH" as the value of "HOME".
		if(std::getenv("HOME")) {
			CONFIG_PATH = std::strcat(std::getenv("HOME"), "/.config/openwars");
		}
		
		// We do the same but with "APPDATA".
		if(std::getenv("APPDATA")) {
			CONFIG_PATH = std::strcat(std::getenv("APPDATA"), "/openwars");
		}
		
		// If "CONFIG_DATA" wasn't set, we return.
		if(CONFIG_PATH == nullptr) {
			return -1;
		}
		
		// TODO : Handle errors.
		std::filesystem::create_directory(std::strcat(CONFIG_PATH, "maps"));
		std::filesystem::create_directory(std::strcat(CONFIG_PATH, "content"));
		std::filesystem::create_directory(std::strcat(CONFIG_PATH, "logs"));
		
		return 0;
	};
}; // namespace Config

#endif
