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
