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
#ifndef __openwars__misc__script_parser__hpp__
#define __openwars__misc__script_parser__hpp__

#include "../nuclei.hpp"
#include <string>
#include <vector>
#include <map>

namespace OpenWars {
	namespace ScriptParser {
		typedef struct {
			std::string command;
			std::string body;
			std::map<std::string, std::string> properties;
		} command_t;

		typedef struct {
			bool success;
			std::string id;
			std::vector<command_t> commands;
			std::vector<std::string> texts;
		} row_t;

		// Initialize the script parser.
		int init(const char **err);
		void deinit(void);

		// Load and parse the script.
		int load_script(const char *script, const char **err);
		void deinit_script(void);

		row_t read_row(unsigned int row, const char **err);
		row_t read_row(const char *row, const char **err);
		row_t read_row(std::string row, const char **err);
	};
};

#endif
