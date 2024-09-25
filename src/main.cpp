#include "io/log.hpp"
#include "logic/task/task.hpp"
#include "misc/auditor.hpp"
#include "visual/visual.hpp"
#include <cstdlib>

const int screen_width = 1280;
const int screen_height = 720;

OpenWars::Tasks::King king;

void ciao(int code) {
	OpenWars::deinit_video();
	king.deinit_pawns();
	OpenWars::deinit_auditor();

	std::exit(code);
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

	while(OpenWars::should_close_window() == false) {
		OpenWars::init_frame(&err);

		// [TODO]

		OpenWars::swap_buffers(&err);
	};

	ciao(0);
}
