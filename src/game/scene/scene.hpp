#ifndef __openwars__game__scene__scene__h__
#define __openwars__game__scene__scene__h__

namespace OpenWars {
	// Enum for identifing different game scenes
	typedef enum {
		// Loading screen between scenes or boot screen
		// ! Never actually shown, for reference purposes only
		LOADING = 0,
		// Main menu screen
		MAIN = 1,
		// In-game music
		GAME = 2
	} Scenes;

	class SceneController {
		public:
		OpenWars::Scenes currentScene = OpenWars::Scenes::LOADING;

		SceneController();
		~SceneController();
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