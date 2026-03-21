#pragma once

#include "terrain.hpp"
#include "../../core/vector.hpp"
#include <functional>
#include <vector>

namespace OpenWars::Game {
    class Unit;

    class Map {
        private:
        int width;
        int height;
        std::vector<Terrain> tiles;

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

        Terrain* getTerrain(int x, int y);
        const Terrain* getTerrain(int x, int y) const;
        const Terrain* getTerrain(const Vector2& pos) const;

        void setTerrain(int x, int y, TileTypeID id);
        void setTerrain(const Vector2& pos, TileTypeID id);

        int getMovementCost(
            const Vector2& pos,
            MovementType movementType
        ) const;
        bool isPassable(const Vector2& pos, MovementType movementType) const;

        std::vector<Vector2> getTilesByType(TileTypeID id) const;

        void fillRectangle(int x, int y, int w, int h, TileTypeID id);

        void clear();

        float getAverageDefense(int x, int y, int radius) const;

        void forEachTile(
            std::function<void(int, int, const Terrain*)> callback
        ) const;
    };
} // namespace OpenWars::Game