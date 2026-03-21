#include "tiletype.hpp"

namespace OpenWars::Game {
    TileRegistry& TileRegistry::get() {
        static TileRegistry inst;
        return inst;
    }

    TileTypeID TileRegistry::registerTile(TileDefinition def) {
        TileTypeID id = nextID++;
        tiles.emplace(id, std::move(def));
        return id;
    }

    const TileDefinition* TileRegistry::find(TileTypeID id) const {
        auto it = tiles.find(id);
        return it != tiles.end() ? &it->second : nullptr;
    }

    const TileDefinition& TileRegistry::lookup(TileTypeID id) const {
        return tiles.at(id);
    }
} // namespace OpenWars::Game