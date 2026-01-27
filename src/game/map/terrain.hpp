#pragma once
#include "../units/unit.hpp"

namespace OpenWars::Game {
    enum class TerrainType {
        Plain,
        Road,
        Wood,
        Mountain,
        River,
        Coast,
        Sea,
        City,
        Factory,
        Airport,
        Port,
        HQ,
        CommTower,
        Lab,
        Pipe,
        Silo
    };

    class Terrain {
        TerrainType type;
        int defenseStars;

      public:
        Terrain(TerrainType type, int defense)
            : type(type)
            , defenseStars(defense) {
        }

        int getDefenseStars() const {
            return defenseStars;
        }
        int getMovementCost(MovementType movementType);
        bool isPassable(MovementType movementType) {
            return getMovementCost(movementType) >= 0;
        }
        TerrainType getType() const {
            return type;
        }
        void render();
    };
} // namespace OpenWars::Game