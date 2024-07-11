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
