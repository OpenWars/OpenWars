#include "io/log.hpp"
#include "logic/task/task.hpp"
#include "misc/auditor.hpp"
#include "visual/visual.hpp"
#include "io/clay.hpp"
#include <cstdlib>

const int screen_width = 1280;
const int screen_height = 720;

OpenWars::Tasks::King king;

void ciao(int code) {
	OpenWars::Clay::deinit();
	OpenWars::deinit_video();
	king.deinit_pawns();
	OpenWars::deinit_auditor();

	std::exit(code);
};

namespace Raylib {
	#include <raylib.h>
};

int main(void) {
	const char *err = nullptr;

	if(OpenWars::init_auditor(&err) < 0) {
		OpenWars::log_error("Couldn't initialize Auditor: %s\n", err);
		ciao(1);
	}

	if(king.init_pawns(2, &err) < 0) {
		OpenWars::log_error("Couldn't initialize King: %s\n", err);
		ciao(1);
	}

	if(OpenWars::init_video(screen_width, screen_height, "OpenWars", &err) < 0) {
		OpenWars::log_error("Couldn't initialize the window: %s\n", err);
		ciao(1);
	}

	if(OpenWars::Clay::init(&err) < 0) {
		OpenWars::log_error("Couldn't initialize ClayUI: %s\n", err);
		ciao(1);
	}

	Raylib::Camera2D cam;
	cam.target = Raylib::Vector2 { 0.0f, 0.0f };
	cam.offset = Raylib::Vector2 { 0.0f, 0.0f };
	cam.rotation = 0.0f;
	cam.zoom = 1.0f;

	Clay_LayoutConfig layoutElement = Clay_LayoutConfig { .padding = {5} };

	while(OpenWars::should_close_window() == false) {
		OpenWars::init_frame(&err);

		// [TODO]
		OpenWars::Clay::update();
		OpenWars::Clay::start_frame();

		// [TODO]

		OpenWars::Clay::end_frame(&cam);
		// [TODO]

		OpenWars::swap_buffers(&err);
	};

	ciao(0);
}
