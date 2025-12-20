#pragma once

#include "map.hpp"
#include <memory>

namespace OpenWars::Game {
    /**
     * Procedural map generation utilities for Advance Wars style maps.
     */
    class MapGenerator {
      public:
        /**
         * Generates a completely random map.
         * @param width Map width in tiles
         * @param height Map height in tiles
         * @return Generated map
         */
        static std::unique_ptr<Map> generateRandom(int width, int height);

        /**
         * Generates a map with islands.
         * @param width Map width in tiles
         * @param height Map height in tiles
         * @param islandCount Number of islands to generate
         * @return Generated map
         */
        static std::unique_ptr<Map>
        generateIslandMap(int width, int height, int islandCount = 5);

        /**
         * Generates a map with rivers and valleys.
         * @param width Map width in tiles
         * @param height Map height in tiles
         * @return Generated map
         */
        static std::unique_ptr<Map> generateRiverMap(int width, int height);

        /**
         * Generates a map with mountains and varied terrain.
         * @param width Map width in tiles
         * @param height Map height in tiles
         * @return Generated map
         */
        static std::unique_ptr<Map> generateMountainMap(int width, int height);

        /**
         * Generates a simple test map with basic terrain.
         * @param width Map width in tiles
         * @param height Map height in tiles
         * @return Generated map
         */
        static std::unique_ptr<Map> generateTestMap(int width, int height);

        /**
         * Generates a symmetrical map suitable for 2-player battles.
         * @param width Map width in tiles
         * @param height Map height in tiles
         * @return Generated map
         */
        static std::unique_ptr<Map>
        generateSymmetricalMap(int width, int height);

      private:
        /**
         * Perlin-like noise function for terrain generation.
         */
        static float perlinNoise(int x, int y, int seed);

        /**
         * Smoothstep interpolation function.
         */
        static float smoothstep(float t);

        /**
         * Applies smoothing to terrain to create more natural features.
         */
        static void smoothTerrain(Map* map, int radius = 1);

        /**
         * Adds scattered structures like cities, airports, etc.
         */
        static void addStructures(
            Map* map,
            int count,
            TerrainType type = TerrainType::City
        );

        /**
         * Adds roads connecting structures.
         */
        static void addRoads(Map* map);
    };

} // namespace OpenWars::Game
