#pragma once

namespace OpenWars::Game {
    enum class TerrainType {
        Plain,
        Road,
        Wood,
        Mountain,
        River,
        Beach,
        Sea,
        Shoal,
        Reef,
        City,
        Base,
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
        TerrainType getType() const {
            return type;
        }
    };
} // namespace OpenWars::Game