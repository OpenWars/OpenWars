#include "defs.hpp"
#include "game/scene/scene.hpp"
#include "logic/lua/lua.hpp"
#include "config/config.hpp"
#include "logic/task/task.hpp"
#include "visual/visual.hpp"

#include <cstdio>

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	OpenWars::SceneController sceneController;
	OpenWars::Config config;

	if(config.load() < 0) {
		std::fprintf(stderr, "{CONFIG_LOAD_ERROR} %s\n", config.get_error());
		return 1;
	}

	OpenWars::Tasks::King tasks_king;

	if(tasks_king.init_pawns() < 0) {
		std::fprintf(stderr, "{TASK_SCHEDULER_CREATE_ERROR} %s\n", tasks_king.get_error());
		return 1;
	}

	OpenWars::init_video(screenWidth, screenHeight, "OpenWars");

	while(OpenWars::should_close_window() == false) {
		OpenWars::init_frame();

		// [TODO]

		OpenWars::swap_buffers();
	};

	OpenWars::deinit_video();
	tasks_king.deinit_pawns();

	return 0;
}
