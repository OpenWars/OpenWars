#include "defs.hpp"
#include "game/scene/scene.hpp"
#include "logic/lua/lua.hpp"
#include "config/config.hpp"
#include "logic/task/task.hpp"

#include <cstdio>

namespace Raylib {
	#include <raylib.h>
};

int main(void) {
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

	const int screenWidth = 800;
	const int screenHeight = 450;

	Raylib::InitWindow(screenWidth, screenHeight, "OpenWars");

	Raylib::Camera2D camera = {
		{ 0.0, 0.0 },
		{ 0.0, 0.0 },
		0.0,
		1.0
	};

	Raylib::SetTargetFPS(60);

	while(Raylib::WindowShouldClose() != true) {
		Raylib::BeginDrawing();
		Raylib::ClearBackground(Raylib::BLACK);

		Raylib::BeginMode2D(camera);

		// [TODO]

		Raylib::EndMode2D();
		Raylib::EndDrawing();
	};

	Raylib::CloseWindow();
	tasks_king.deinit_pawns();

	return 0;
}
