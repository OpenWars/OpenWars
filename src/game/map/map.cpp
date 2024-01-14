#include "map.h"
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

void Map::draw(Map map){
    
}