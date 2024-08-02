/**
 *
   ___                __        __             
  / _ \ _ __   ___ _ _\ \      / /_ _ _ __ ___ 
 | | | | '_ \ / _ \ '_ \ \ /\ / / _` | '__/ __|
 | |_| | |_) |  __/ | | \ V  V / (_| | |  \__ \
  \___/| .__/ \___|_| |_|\_/\_/ \__,_|_|  |___/
       |_|     

Copyright (C) 2024 OpenWars Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __openwars__config__config__cxx__
#define __openwars__config__config__cxx__

#include "config.hpp"

#include <filesystem>
#include <cstdlib>
#include <cstring>
#include <new>

namespace OpenWars {
	Config::Config(void) {};

	Config::~Config(void) {
		delete[] profile_path;
	}

	const char *Config::get_error(void) {
		return err_str;
	};

	int Config::get_profile_path(void) {
		char *tmp_buff = nullptr;
		const char *ap_str = nullptr;

		if(tmp_buff == nullptr) {
			tmp_buff = std::getenv("HOME");
			ap_str = "/.config/openwars/";
		}

		if(tmp_buff == nullptr) {
			tmp_buff = std::getenv("APPDATA");
			ap_str = "/openwars/";
		}

		if((tmp_buff != nullptr) && (ap_str != nullptr)) {
			try {
				profile_path = new char[std::strlen(tmp_buff) + 256];
			} catch(std::bad_alloc& e) {
				err_str = "Couldn't allocate enough memory";
				return -1;
			};

			std::strcat(profile_path, tmp_buff);
			std::strcat(profile_path, ap_str);

			return 0;
		}

		err_str = "Couldn't find a suitable profile path";

		return 1;
	};

	int Config::create_dirs(const char *path) {
		try {
			std::filesystem::create_directories(path);
		} catch(std::filesystem::filesystem_error& e) {
			err_str = e.what();
			return 1;
		};

		return 0;
	};

	int Config::create_blank_profile(void) {
		char *tmp_buff = nullptr;

		try {
			tmp_buff = new char[std::strlen(profile_path) + 256];
		} catch(std::bad_alloc& e) {
			err_str = "Couldn't allocate enough memory";
			return -1;
		};

		std::strcpy(tmp_buff, profile_path);
		std::strcat(tmp_buff, "maps");
		if(create_dirs(tmp_buff) < 0) return -1;

		std::strcpy(tmp_buff, profile_path);
		std::strcat(tmp_buff, "textures");
		if(create_dirs(tmp_buff) < 0) return -1;

		std::strcpy(tmp_buff, profile_path);
		std::strcat(tmp_buff, "logs");
		if(create_dirs(tmp_buff) < 0) return -1;

		delete[] tmp_buff;

		return 0;
	};

	int Config::load(void) {
		if(get_profile_path() < 0) return -1;
		if(create_blank_profile() < 0) return -1;

		return 0;
	};
};

#endif
