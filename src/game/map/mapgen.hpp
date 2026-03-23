#pragma once

#include "map.hpp"
#include <memory>

namespace OpenWars::Game {
    class MapGenerator {
        public:
        static std::unique_ptr<Map> generateRandom(int width, int height);
        static std::unique_ptr<Map>
        generateIslandMap(int width, int height, int islandCount = 5);
        static std::unique_ptr<Map> generateRiverMap(int width, int height);
        static std::unique_ptr<Map> generateMountainMap(int width, int height);
        static std::unique_ptr<Map> generateTestMap(int width, int height);
        static std::unique_ptr<Map>
        generateSymmetricalMap(int width, int height);

        private:
        static float perlinNoise(int x, int y, int seed);
        static void smoothTerrain(Map* map, int radius = 1);
        static void addStructures(Map* map, int count, TileTypeID type);
        static void addRoads(Map* map);
    };
} // namespace OpenWars::Game