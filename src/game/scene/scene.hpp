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
