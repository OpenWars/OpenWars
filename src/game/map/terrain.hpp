#pragma once

namespace OpenWars::Game {
    enum class MovementType;

    enum class TerrainType {
        Plain,
        Road,
        Bridge,
        Woods,
        Mountain,
        HighMountain,
        River,
        Coast,
        Sea,
        Reef,
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

        static int defaultDefense(TerrainType type) {
            switch(type) {
            case TerrainType::Plain:
            case TerrainType::Reef:
                return 1;
            case TerrainType::Woods:
                return 2;
            case TerrainType::City:
            case TerrainType::Factory:
            case TerrainType::Airport:
            case TerrainType::Port:
            case TerrainType::CommTower:
            case TerrainType::Lab:
            case TerrainType::Silo:
                return 3;
            case TerrainType::Mountain:
            case TerrainType::HQ:
                return 4;
            default:
                return 0;
            }
        }

        public:
        explicit Terrain(TerrainType type)
            : type(type)
            , defenseStars(defaultDefense(type)) {
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