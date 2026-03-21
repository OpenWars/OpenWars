#include "terrain.hpp"

namespace OpenWars::Game {
    const TileDefinition* Terrain::getDef() const {
        return TileRegistry::get().find(typeId);
    }

    int Terrain::getDefenseStars() const {
        const TileDefinition* def = getDef();
        return def ? def->defenseStars : 0;
    }

    int Terrain::getMovementCost(MovementType type) const {
        const TileDefinition* def = getDef();
        if(!def)
            return -1;
        auto it = def->movementCosts.find(type);
        return it != def->movementCosts.end() ? it->second : -1;
    }
} // namespace OpenWars::Game