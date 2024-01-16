#include <unordered_map>
#include "../../config/config.h"
#include "raylib.h"
#include "map.h"
#include <string>

namespace Map {
    auto tilesIndex = std::unordered_map<std::string, Texture2D>();

    void load_images(){
        // Hardcode because why not

        tilesIndex[0] = LoadTextureFromImage(LoadImage((std::string(Config::CONFIG_PATH) + "/content/" + "grasstile.png").c_str()));
    }   
}