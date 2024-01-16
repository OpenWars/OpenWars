#include "tiles.cpp"
#include "../../config/config.h"
#include <fstream>
#include <sstream>
#include <vector>

Map::Map Map::parse(std::string name) {
    std::string text;
    std::ifstream rawmap(std::string(Config::CONFIG_PATH) + "/maps/" +
                         std::string(name));

    std::vector<std::vector<std::string>> mapArray;

    std::string line;
    if (rawmap.is_open()) {
        while (std::getline(rawmap, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;

            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }

            mapArray.push_back(row);
        }
        rawmap.close();
    }

    return Map{static_cast<int>(mapArray[0].size()),
               static_cast<int>(mapArray.size()), mapArray};
}

void Map::draw(Map map, int screenWidth, int screenHeight) {
    int rows = map.height;
    int cols = map.width;
    int cellWidth = screenWidth / cols;
    int cellHeight = screenHeight / rows;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Calculate cell position
            int xPos = j * cellWidth;
            int yPos = i * cellHeight;

            // Get the image index for the current cell
            std::string imageIndex = map.tiles[i][j];

            // Draw image in the cell
            DrawTextureRec(
                Texture2D(tilesIndex[imageIndex]),
                {0, 0, static_cast<float>(tilesIndex[imageIndex].width),
                 static_cast<float>(-tilesIndex[imageIndex].height)},
                {static_cast<float>(xPos), static_cast<float>(yPos)}, WHITE);
        }
    }
}