#include <iostream>

namespace Raylib {
	#include <raylib.h>
};

int main(void) {
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
	}

	Raylib::CloseWindow();

	return 0;
}
