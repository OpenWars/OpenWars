#include "terrain.hpp"

namespace OpenWars::Game {

    namespace {
        constexpr int I = static_cast<int>(MovementType::Infantry);
        constexpr int M = static_cast<int>(MovementType::Mech);
        constexpr int T = static_cast<int>(MovementType::Tire);
        constexpr int R = static_cast<int>(MovementType::Tread);
        constexpr int A = static_cast<int>(MovementType::Air);
        constexpr int S = static_cast<int>(MovementType::Sea);
        constexpr int L = static_cast<int>(MovementType::Lander);
        constexpr int P = static_cast<int>(MovementType::Pipe);
    } // namespace

    TerrainRegistry::TerrainRegistry() {
        auto reg = [&](TerrainType type,
                       int def,
                       int infantryCost,
                       int mechCost,
                       int tireCost,
                       int treadCost,
                       int airCost,
                       int seaCost,
                       int landerCost,
                       int pipeCost) {
            TerrainDefinition d;
            d.defenseStars = def;
            d.costs[I] = infantryCost;
            d.costs[M] = mechCost;
            d.costs[T] = tireCost;
            d.costs[R] = treadCost;
            d.costs[A] = airCost;
            d.costs[S] = seaCost;
            d.costs[L] = landerCost;
            d.costs[P] = pipeCost;
            definitions[static_cast<int>(type)] = d;
        };

        // all hail the unit registry matrix!
        reg(TerrainType::Plain, 1, 1, 1, 1, 1, 1, -1, -1, -1);
        reg(TerrainType::Road, 0, 1, 1, 1, 1, 1, -1, -1, -1);
        reg(TerrainType::Bridge, 0, 1, 1, 1, 1, 1, -1, -1, -1);
        reg(TerrainType::Woods, 2, 2, 2, 3, 2, 1, -1, -1, -1);
        reg(TerrainType::Mountain, 4, 4, 4, -1, -1, 1, -1, -1, -1);
        reg(TerrainType::HighMountain, 4, 4, 4, -1, -1, 1, -1, -1, -1);
        reg(TerrainType::River, 0, -1, -1, -1, -1, 1, -1, -1, -1);
        reg(TerrainType::Coast, 0, 1, 1, 1, 1, 1, 1, 1, -1);
        reg(TerrainType::Sea, 0, -1, -1, -1, -1, 1, 1, 1, -1);
        reg(TerrainType::Reef, 1, -1, -1, -1, -1, 1, 1, 1, -1);
        reg(TerrainType::City, 3, 1, 1, 1, 1, 1, -1, -1, -1);
        reg(TerrainType::Factory, 3, 1, 1, 1, 1, 1, -1, -1, -1);
        reg(TerrainType::Airport, 3, 1, 1, 1, 1, 1, -1, -1, -1);
        reg(TerrainType::Port, 3, 1, 1, 1, 1, 1, 1, 1, -1);
        reg(TerrainType::HQ, 4, 1, 1, 1, 1, 1, -1, -1, -1);
        reg(TerrainType::CommTower, 3, 1, 1, 1, 1, 1, -1, -1, -1);
        reg(TerrainType::Lab, 3, 1, 1, 1, 1, 1, -1, -1, -1);
        reg(TerrainType::Silo, 3, 2, 2, -1, -1, 1, -1, -1, -1);
        reg(TerrainType::Pipe, 0, 1, 1, -1, -1, 1, -1, -1, 1);
    }

    TerrainRegistry& TerrainRegistry::get() {
        static TerrainRegistry instance;
        return instance;
    }

    void TerrainRegistry::registerDefinition(
        TerrainType type,
        TerrainDefinition def
    ) {
        definitions[static_cast<int>(type)] = def;
    }

    const TerrainDefinition* TerrainRegistry::find(TerrainType type) const {
        auto it = definitions.find(static_cast<int>(type));
        return it != definitions.end() ? &it->second : nullptr;
    }

    int Terrain::getDefenseStars() const {
        const TerrainDefinition* def = TerrainRegistry::get().find(type);
        return def ? def->defenseStars : 0;
    }

    int Terrain::getMovementCost(MovementType movementType) const {
        const TerrainDefinition* def = TerrainRegistry::get().find(type);
        return def ? def->movementCost(movementType) : -1;
    }

    void Terrain::render() {
    }

} // namespace OpenWars::Game