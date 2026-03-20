#pragma once

#include <unordered_map>

namespace OpenWars::Game {

    enum class MovementType {
        Infantry,
        Mech,
        Tire,
        Tread,
        Air,
        Sea,
        Lander,
        Pipe
    };

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

    struct TerrainDefinition {
        int defenseStars = 0;
        // Indexed by static_cast<int>(MovementType). -1 = impassable.
        // Array size matches MovementType value count (8).
        int costs[8];

        TerrainDefinition() {
            for(int& c : costs)
                c = -1;
        }

        int movementCost(MovementType mt) const {
            return costs[static_cast<int>(mt)];
        }

        TerrainDefinition& setCost(MovementType mt, int cost) {
            costs[static_cast<int>(mt)] = cost;
            return *this;
        }
    };

    class TerrainRegistry {
        public:
        static TerrainRegistry& get();

        void registerDefinition(TerrainType type, TerrainDefinition def);
        const TerrainDefinition* find(TerrainType type) const;

        private:
        TerrainRegistry();
        std::unordered_map<int, TerrainDefinition> definitions;
    };

    class Terrain {
        TerrainType type;

        public:
        explicit Terrain(TerrainType type)
            : type(type) {
        }

        int getDefenseStars() const;
        int getMovementCost(MovementType movementType) const;
        bool isPassable(MovementType movementType) const {
            return getMovementCost(movementType) >= 0;
        }
        TerrainType getType() const {
            return type;
        }
        void render();
    };

} // namespace OpenWars::Game