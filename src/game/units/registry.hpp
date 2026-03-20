#pragma once

#include "definition.hpp"
#include "../../core/vector.hpp"
#include <unordered_map>
#include <memory>

namespace OpenWars::Game {

    class Unit;

    class UnitRegistry {
        public:
        static UnitRegistry& get();

        void registerDefinition(UnitDefinition def);
        const UnitDefinition* find(const std::string& id) const;

        std::unique_ptr<Unit>
        create(const std::string& id, int playerID, Vector2 startPos) const;

        private:
        UnitRegistry();
        std::unordered_map<std::string, UnitDefinition> definitions;

        static void registerBuiltins(UnitRegistry& reg);
    };

} // namespace OpenWars::Game