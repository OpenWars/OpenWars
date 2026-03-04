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

        /*
         * Flat 1D storage: tiles[y * width + x]
         *  Eliminates per-row heap allocations and pointer chasing that came
         * with vector<vector<>>, giving better spatial locality.
         */
        std::vector<std::unique_ptr<Terrain>> tiles;

        int index(int x, int y) const {
            return y * width + x;
        }

      public:
        Map(int width, int height);
        virtual ~Map() = default;

        int getWidth() const;
        int getHeight() const;

        bool isInBounds(const Vector2& pos) const;
        bool isInBounds(int x, int y) const;

        Terrain* getTerrain(int x, int y) const;
        Terrain* getTerrain(const Vector2& pos) const;

        void setTerrain(int x, int y, TerrainType type, int defense = 0);
        void setTerrain(const Vector2& pos, TerrainType type, int defense = 0);

        int
        getMovementCost(const Vector2& pos, MovementType movementType) const;
        bool isPassable(const Vector2& pos, MovementType movementType) const;

        std::vector<Vector2> getTilesByType(TerrainType type) const;

        void fillRectangle(
            int x,
            int y,
            int w,
            int h,
            TerrainType type,
            int defense = 0
        );

        void clear();

        float getAverageDefense(int x, int y, int radius) const;

        void
        forEachTile(std::function<void(int, int, Terrain*)> callback) const;
    };
} // namespace OpenWars::Game