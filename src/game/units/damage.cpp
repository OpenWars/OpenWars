#include "damage.hpp"
#include "../co/co.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace OpenWars::Game {

    int DamageCalculator::calculateDamage(
        const Unit* attacker,
        const Unit* defender,
        const Terrain* defenderTerrain,
        bool includeRandom
    ) {
        // Get base damage from damage table
        int baseDamage = attacker->calculateBaseDamage(defender);

        if(baseDamage == DamageTable::NO_DAMAGE) {
            return 0;
        }

        // AWBW Formula:
        // Damage = (BaseDamage × AttackerHP / 10) × (AttackerAttack / 100) ×
        // (DefenderDefense / 100) + Random

        // Step 1: Scale by attacker HP (damage scales with HP in AW)
        float damage = baseDamage * (attacker->getHP() / 10.0f);

        // Step 2: Apply attacker's attack modifier
        int attackMod = getAttackModifier(attacker);
        damage *= (attackMod / 100.0f);

        // Step 3: Apply defender's defense modifier
        int defenseMod = getDefenseModifier(defender, defenderTerrain);
        damage *= (100.0f / defenseMod);

        // Step 4: Add random component (0-9)
        if(includeRandom) {
            damage += getRandomDamage();
        }

        // Apply luck from CO
        if(attacker->getCO()) {
            int luck = attacker->getCO()->getLuck();
            if(luck > 0 && includeRandom) {
                int luckDamage = std::rand() % (luck + 1);
                damage += luckDamage;
            }
        }

        // Round and clamp
        int finalDamage = (int)(damage + 0.5f);
        finalDamage = std::max(0, finalDamage);
        finalDamage = std::min(defender->getHP(), finalDamage);

        return finalDamage;
    }

    int DamageCalculator::getAttackModifier(const Unit* unit) {
        int modifier = 100; // Base 100%

        // Apply CO bonuses
        if(unit->getCO()) {
            modifier = unit->getCO()->getAttackModifier();
        }

        return modifier;
    }

    int DamageCalculator::getDefenseModifier(
        const Unit* unit,
        const Terrain* terrain
    ) {
        int modifier = 100; // Base 100%

        // Apply CO defense bonus
        if(unit->getCO()) {
            modifier = unit->getCO()->getDefenseModifier();
        }

        // Apply terrain defense
        if(terrain) {
            int terrainBonus = getTerrainDefenseBonus(terrain);

            // In AWBW, terrain defense is multiplicative
            // Each star adds roughly 10% defense (reduces damage by 10%)
            // Formula: Defense = 100 + (Stars × 10)
            modifier = (modifier * terrainBonus) / 100;
        }

        return modifier;
    }

    int DamageCalculator::getTerrainDefenseBonus(const Terrain* terrain) {
        if(!terrain)
            return 100;

        int stars = terrain->getDefenseStars();

        // Each star reduces incoming damage
        // 1 star = 110% effective defense (reduces damage by ~9%)
        // 2 stars = 120% effective defense (reduces damage by ~17%)
        // 3 stars = 130% effective defense (reduces damage by ~23%)
        // 4 stars = 140% effective defense (reduces damage by ~29%)
        return 100 + (stars * 10);
    }

    bool DamageCalculator::canUnitAttackTarget(
        const Unit* attacker,
        const Unit* target
    ) {
        if(!attacker || !target)
            return false;

        // Check if base damage exists
        int baseDamage = attacker->calculateBaseDamage(target);
        return baseDamage != DamageTable::NO_DAMAGE;
    }

    int DamageCalculator::getRandomDamage() {
        // Random component from 0 to 9
        static bool seeded = false;
        if(!seeded) {
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
            seeded = true;
        }

        return std::rand() % 10;
    }

} // namespace OpenWars::Game