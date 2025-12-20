#pragma once

#include "terrain.hpp"
#include "../../core/vector.hpp"
#include "../units/unit.hpp"
#include <vector>
#include <memory>
#include <functional>

namespace OpenWars::Game {
    class Map {
      private:
        int width;
        int height;
        std::vector<std::vector<std::unique_ptr<Terrain>>> tiles;

      public:
        /**
         * Creates a map with specified dimensions.
         * @param width The width of the map in tiles.
         * @param height The height of the map in tiles.
         */
        Map(int width, int height);

        virtual ~Map() = default;

        /**
         * Gets the width of the map in tiles.
         */
        int getWidth() const;

        /**
         * Gets the height of the map in tiles.
         */
        int getHeight() const;

        /**
         * Checks if a position is within map bounds.
         */
        bool isInBounds(const Vector2& pos) const;

        /**
         * Checks if a position is within map bounds.
         */
        bool isInBounds(int x, int y) const;

        /**
         * Gets the terrain at a specific position.
         */
        Terrain* getTerrain(int x, int y) const;

        /**
         * Gets the terrain at a vector position.
         */
        Terrain* getTerrain(const Vector2& pos) const;

        /**
         * Sets the terrain at a specific position.
         */
        void setTerrain(int x, int y, TerrainType type, int defense = 0);

        /**
         * Sets the terrain at a vector position.
         */
        void setTerrain(const Vector2& pos, TerrainType type, int defense = 0);

        /**
         * Gets terrain movement cost for a specific unit movement type.
         * Returns the cost to move across this terrain, or -1 if impassable.
         */
        int
        getMovementCost(const Vector2& pos, MovementType movementType) const;

        /**
         * Checks if a terrain is passable for a specific movement type.
         */
        bool isPassable(const Vector2& pos, MovementType movementType) const;

        /**
         * Gets all tiles of a specific type.
         */
        std::vector<Vector2> getTilesByType(TerrainType type) const;

        /**
         * Fills a rectangular area with a specific terrain.
         */
        void fillRectangle(
            int x,
            int y,
            int w,
            int h,
            TerrainType type,
            int defense = 0
        );

        /**
         * Clears the map, setting all tiles to Plain terrain.
         */
        void clear();

        /**
         * Gets average defense rating of terrain in a region.
         */
        float getAverageDefense(int x, int y, int radius) const;

        /**
         * Iterates over all tiles and applies a function.
         */
        void
        forEachTile(std::function<void(int, int, Terrain*)> callback) const;
    };

} // namespace OpenWars::Game
