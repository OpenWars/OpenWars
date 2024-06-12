#ifndef __openwars__game__scene__scene__h__
#define __openwars__game__scene__scene__h__

namespace Scene {
	// Enum for identifing different game scenes
	enum Scenes {
		// Loading screen between scenes or boot screen
		// ! Never actually shown, for reference purposes only
		LOADING = 0,
		// Main menu screen
		MAIN = 1,
		// In-game music
		GAME = 2
	};

	// Scenes::MAIN
	class MainScene {
		public:
		const char* playerName;

		MainScene(const char* playerName);
		~MainScene();

		void render();
	};

	// TODO Scenes::GAME - Until map format is finished.
}

#endif