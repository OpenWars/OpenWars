#pragma once
#include <array>
#include <string>
#include <unordered_map>
#include <vector>
#include "../movement.hpp"

namespace OpenWars::Game {
    using TileTypeID = int;

    struct TileSprite {
        int row = 0, col = 0; // 1-based spritesheet coords; row=0 means absent
        int altRow = 0, altCol = 0; // odd-y variant; 0 = same as primary
        int frameCount = 1;
        float animSpeed = 0.0f;
        int frameStride = 1;
    };

    struct TileConnectionSprites {
        struct Entry {
            int row = 0, col = 0, rotation = 0;
        };
        // Indexed by 4-bit neighbor mask: N=bit0 S=bit1 E=bit2 W=bit3
        std::array<Entry, 16> byMask{};
    };

    struct TileDefinition {
        std::string name;
        int defenseStars = 0;
        std::unordered_map<MovementType, int> movementCosts;

        TileSprite sprite;
        TileSprite underlay; // drawn beneath in foreground pass; row=0 = absent
        TileSprite overlay;  // drawn one tile-height above; row=0 = absent

        bool foreground =
            false; // rendered in foreground pass with underlay beneath

        // Connection rendering: tiles in the same group (and listed in
        // connectsTo) connect to each other, driving the connection sprite
        // selection.
        int connectionGroup = 0; // 0 = no connection rendering
        bool diagonalConnectionFallback =
            false; // check diagonals when cardinal mask = 0
        std::vector<int> connectsTo;
        TileConnectionSprites connectionSprites;
    };

    class TileRegistry {
        public:
        static TileRegistry& get();

        TileTypeID registerTile(TileDefinition def);
        const TileDefinition* find(TileTypeID id) const;
        const TileDefinition& lookup(TileTypeID id) const;

        private:
        TileRegistry() = default;
        std::unordered_map<TileTypeID, TileDefinition> tiles;
        TileTypeID nextID = 1;
    };
} // namespace OpenWars::Game