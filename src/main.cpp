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
#include "defs.hpp"
#include "game/scene/scene.hpp"
#include "logic/lua/lua.hpp"
#include "config/config.hpp"
#include "logic/task/task.hpp"
#include "visual/visual.hpp"
#include "misc/auditor.hpp"
#include "io/log.hpp"

#include <cstdio>

using namespace OpenWars;

int main(void) {
	ErrorOr<void> ve = init_auditor();
	if(ve.error) {
		log_error("Couldn't initialize the auditor: %s\n", ve.error);
		return 1;
	}

	const int screenWidth = 800;
	const int screenHeight = 450;

	SceneController sceneController;
	Config config;

	if(config.load() < 0) {
		log_error("{CONFIG_LOAD_ERROR} %s\n", config.get_error());
		return 1;
	}

	Tasks::King tasks_king;

	if(tasks_king.init_pawns() < 0) {
		log_error("{TASK_SCHEDULER_CREATE_ERROR} %s\n", tasks_king.get_error());
		return 1;
	}

	init_video(screenWidth, screenHeight, "OpenWars");

	while(should_close_window() == false) {
		init_frame();

		// [TODO]

		swap_buffers();
	};

	deinit_video();
	tasks_king.deinit_pawns();

	deinit_auditor();

	return 0;
}
