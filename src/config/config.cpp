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
#include "../io/files.hpp"
#include "../misc/auditor.hpp"
#include <stdexcept>
#include <unordered_map>
#include <cstring>

namespace OpenWars {
	struct i_config_t {
		char *dir_path = nullptr;
		std::unordered_map<const char *, const char *> keys;
		u64	audit_id = (u64)(-1);
	};

	i_config_t i_conf;

	i8 i_get_config_path(const char **err) {
		if(*err != nullptr)
			return -1;

		const char *env_str = nullptr;
		const char *add_str = nullptr;

		if(env_str == nullptr) {
			env_str = std::getenv("HOME");
			add_str = "/.config/openwars/";
		}

		if(env_str == nullptr) {
			env_str = std::getenv("APPDATA");
			add_str = "/openwars/";
		}

		if(env_str == nullptr) {
			env_str = ".";
			add_str = "/";
		}

		u64 len = 1;
		len += std::strlen(env_str);
		len += std::strlen(add_str);

		char *path = valloc<char>(len, err);
		if(path == nullptr)
			return -1;

		i_conf.dir_path = path;

		return 0;
	};

	i8 i_create_blank_config(const char **err) {
		char *path = valloc<char>(std::strlen(i_conf.dir_path) + 256, err);
		if(path == nullptr)
			return -1;

		std::strcpy(path, i_conf.dir_path);
		std::strcat(path, "maps");
		if(create_directories(path, err) < 0)
			return -1;

		std::strcpy(path, i_conf.dir_path);
		std::strcat(path, "textures");
		if(create_directories(path, err) < 0)
			return -1;

		std::strcpy(path, i_conf.dir_path);
		std::strcat(path, "logs");
		if(create_directories(path, err) < 0)
			return -1;

		vfree(path);

		return 0;
	};

	i8 load_config(const char **err) {
		if(*err != nullptr)
			return -1;

		free_config();

		i_conf.audit_id = audit(AUDITOR_RESOURCES::MISC, "load_config", err);
		if(*err != nullptr) {
			free_config();
			return -1;
		}

		if(i_get_config_path(err) < 0) {
			free_config();
			return -1;
		}

		if(i_create_blank_config(err) < 0) {
			free_config();
			return -1;
		}

		// [TODO] : Load keys->values.

		return 0;
	};

	void free_config(void) {
		if(i_conf.dir_path != nullptr) {
			vfree(i_conf.dir_path);
			i_conf.dir_path = nullptr;

			const char *err = nullptr;
			(void)deaudit(i_conf.audit_id, &err);
			(void)err;
		}

		i_conf.keys.clear();
	};

	const char *get_config_string(const char *key, const char **err) {
		if(*err != nullptr)
			return nullptr;

		if(i_conf.dir_path == nullptr) {
			*err = "Configuration isn't loaded";
			return nullptr;
		}
		
		const char *v = nullptr;

		try {
			v = i_conf.keys.at(key);
		} catch(std::out_of_range &e) {
			*err = "Key is not present";
			return nullptr;
		};

		return v;
	};
};

#endif
