#pragma once

#include <string>
#include <unordered_map>

namespace OpenWars::Game {

    class Unit;
    class Terrain;

    class DamageTable {
        public:
        static constexpr int NO_DAMAGE = -1;

        static DamageTable& get();

        void set(
            const std::string& attackerId,
            const std::string& defenderId,
            int damage
        );
        int lookup(
            const std::string& attackerId,
            const std::string& defenderId
        ) const;

        private:
        DamageTable() = default;
        std::unordered_map<std::string, std::unordered_map<std::string, int>>
            chart;
    };

    class DamageCalculator {
        public:
        static int calculateDamage(
            const Unit* attacker,
            const Unit* defender,
            const Terrain* defenderTerrain,
            bool includeRandom = true
        );

        static bool
        canUnitAttackTarget(const Unit* attacker, const Unit* target);

        private:
        static int getAttackModifier(const Unit* unit);
        static int getDefenseModifier(const Unit* unit, const Terrain* terrain);
        static int getTerrainDefenseBonus(const Terrain* terrain);
        static int getRandomDamage();
    };

} // namespace OpenWars::Game