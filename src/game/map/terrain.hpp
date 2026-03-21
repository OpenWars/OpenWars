#pragma once

#include "tiletype.hpp"

namespace OpenWars::Game {
    struct Terrain {
        TileTypeID typeId;

        explicit Terrain(TileTypeID id = 0)
            : typeId(id) {
        }

        const TileDefinition* getDef() const;
        int getDefenseStars() const;
        int getMovementCost(MovementType type) const;
        bool isPassable(MovementType type) const {
            return getMovementCost(type) >= 0;
        }
        void render() {
        }
    };
} // namespace OpenWars::Game