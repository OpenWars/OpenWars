#include "damage.hpp"
#include "unit.hpp"
#include "../co/co.hpp"
#include "../map/terrain.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace OpenWars::Game {

    DamageTable& DamageTable::get() {
        static DamageTable instance;
        return instance;
    }

    void DamageTable::set(
        const std::string& attackerId,
        const std::string& defenderId,
        int damage
    ) {
        chart[attackerId][defenderId] = damage;
    }

    int DamageTable::lookup(
        const std::string& attackerId,
        const std::string& defenderId
    ) const {
        auto aIt = chart.find(attackerId);
        if(aIt == chart.end())
            return NO_DAMAGE;

        auto dIt = aIt->second.find(defenderId);
        return dIt != aIt->second.end() ? dIt->second : NO_DAMAGE;
    }

    int DamageCalculator::calculateDamage(
        const Unit* attacker,
        const Unit* defender,
        const Terrain* defenderTerrain,
        bool includeRandom
    ) {
        int baseDamage = attacker->calculateBaseDamage(defender);
        if(baseDamage == DamageTable::NO_DAMAGE)
            return 0;

        // AWBW formula: (base × attackerHP/10) × (atk/100) × (100/def)
        float damage = baseDamage * (attacker->getHP() / 10.0f);
        damage *= getAttackModifier(attacker) / 100.0f;
        damage *= 100.0f / getDefenseModifier(defender, defenderTerrain);

        if(includeRandom)
            damage += getRandomDamage();

        if(attacker->getCO() && includeRandom) {
            int luck = attacker->getCO()->getLuck();
            if(luck > 0)
                damage += std::rand() % (luck + 1);
        }

        int final = (int)(damage + 0.5f);
        return std::max(0, std::min(defender->getHP(), final));
    }

    bool DamageCalculator::canUnitAttackTarget(
        const Unit* attacker,
        const Unit* target
    ) {
        if(!attacker || !target)
            return false;
        return attacker->calculateBaseDamage(target) != DamageTable::NO_DAMAGE;
    }

    int DamageCalculator::getAttackModifier(const Unit* unit) {
        return unit->getCO() ? unit->getCO()->getAttackModifier() : 100;
    }

    int DamageCalculator::getDefenseModifier(
        const Unit* unit,
        const Terrain* terrain
    ) {
        int modifier =
            unit->getCO() ? unit->getCO()->getDefenseModifier() : 100;

        if(terrain)
            modifier = (modifier * getTerrainDefenseBonus(terrain)) / 100;

        return modifier;
    }

    int DamageCalculator::getTerrainDefenseBonus(const Terrain* terrain) {
        return terrain ? 100 + (terrain->getDefenseStars() * 10) : 100;
    }

    int DamageCalculator::getRandomDamage() {
        static bool seeded = false;
        if(!seeded) {
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
            seeded = true;
        }
        return std::rand() % 10;
    }

} // namespace OpenWars::Game