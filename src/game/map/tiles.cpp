#ifndef __openwars__game__map__tiles__cxx__
#define __openwars__game__map__tiles__cxx__

#include "raylib.h"
#include <unordered_map>
#include "../../config/config.h"
#include "map.h"
#include <cstring>
#include <string>

#include <iostream>

namespace Map {
	auto tilesIndex = std::unordered_map<const char *, Texture2D>();
	
	void load_images(void) {
		std::cout<<"RS01  .";
		
		// Hardcoded because :´}_!
		tilesIndex[0] = LoadTextureFromImage(
			LoadImage(
				std::strcat(Config::CONFIG_PATH, "/content/grasstile.png")));
	};
};

/*
namespace Map {
    auto tilesIndex = std::unordered_map<std::string, Texture2D>();

    void load_images(){
        // Hardcode because why not

        tilesIndex[0] = LoadTextureFromImage(LoadImage((std::string(Config::CONFIG_PATH) + "/content/" + "grasstile.png")));
	};
};
*/

#endif
