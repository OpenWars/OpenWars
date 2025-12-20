#pragma once

#include "../map/terrain.hpp"
#include "unit.hpp"
#include <unordered_map>

namespace OpenWars::Game {
    // Base damage chart (attacker type -> defender type -> base damage)
    // Values are percentages (0-999, where 55 = 55%)
    struct DamageTable {
        static const int NO_DAMAGE = -1; // Cannot attack this target

        // Get base damage from attacker to defender
        static int getBaseDamage(UnitType attacker, UnitType defender);

      private:
        // The actual damage table (simplified version)
        static std::unordered_map<UnitType, std::unordered_map<UnitType, int>>
            damageChart;
        static void initializeDamageChart();
    };

    class DamageCalculator {
      public:
        // Main damage calculation using AWBW formula:
        // Damage = (BaseDamage × AttackerHP / 10) × (AttackerAttack / 100) ×
        // (DefenderDefense / 100) + Random Where Random is 0-9
        static int calculateDamage(
            const Unit* attacker,
            const Unit* defender,
            const Terrain* defenderTerrain,
            bool includeRandom = true
        );

        // Calculate attack modifier from CO
        static int getAttackModifier(const Unit* unit);

        // Calculate defense modifier from CO and terrain
        static int getDefenseModifier(const Unit* unit, const Terrain* terrain);

        // Get terrain defense bonus (in percentage, 100 = normal, 110 = +10%)
        static int getTerrainDefenseBonus(const Terrain* terrain);

        // Check if unit can attack target
        static bool
        canUnitAttackTarget(const Unit* attacker, const Unit* target);

      private:
        // Random damage component (0-9)
        static int getRandomDamage();
    };

    // Simplified damage chart initialization
    // This would be much larger in a full implementation
    inline std::unordered_map<UnitType, std::unordered_map<UnitType, int>>
        DamageTable::damageChart = {};

    // Thank you ChatGPT for reading this for me
    inline void DamageTable::initializeDamageChart() {
        if(!damageChart.empty())
            return;

        // Infantry vs various targets
        damageChart[UnitType::Infantry][UnitType::Infantry] = 55;
        damageChart[UnitType::Infantry][UnitType::Mech] = 45;
        damageChart[UnitType::Infantry][UnitType::Recon] = 12;
        damageChart[UnitType::Infantry][UnitType::Tank] = 5;
        damageChart[UnitType::Infantry][UnitType::MediumTank] = 1;
        damageChart[UnitType::Infantry][UnitType::APC] = 14;
        damageChart[UnitType::Infantry][UnitType::Artillery] = 15;
        damageChart[UnitType::Infantry][UnitType::Rockets] = 25;
        damageChart[UnitType::Infantry][UnitType::AntiAir] = 5;
        damageChart[UnitType::Infantry][UnitType::Missiles] = 26;

        // Mech vs various targets (stronger vs vehicles)
        damageChart[UnitType::Mech][UnitType::Infantry] = 65;
        damageChart[UnitType::Mech][UnitType::Mech] = 55;
        damageChart[UnitType::Mech][UnitType::Recon] = 85;
        damageChart[UnitType::Mech][UnitType::Tank] = 55;
        damageChart[UnitType::Mech][UnitType::MediumTank] = 15;
        damageChart[UnitType::Mech][UnitType::APC] = 75;
        damageChart[UnitType::Mech][UnitType::Artillery] = 70;
        damageChart[UnitType::Mech][UnitType::Rockets] = 85;
        damageChart[UnitType::Mech][UnitType::AntiAir] = 65;
        damageChart[UnitType::Mech][UnitType::Missiles] = 85;

        // Recon vs targets
        damageChart[UnitType::Recon][UnitType::Infantry] = 70;
        damageChart[UnitType::Recon][UnitType::Mech] = 65;
        damageChart[UnitType::Recon][UnitType::Recon] = 35;
        damageChart[UnitType::Recon][UnitType::APC] = 45;
        damageChart[UnitType::Recon][UnitType::Artillery] = 45;
        damageChart[UnitType::Recon][UnitType::AntiAir] = 4;

        // Tank vs targets
        damageChart[UnitType::Tank][UnitType::Infantry] = 75;
        damageChart[UnitType::Tank][UnitType::Mech] = 70;
        damageChart[UnitType::Tank][UnitType::Recon] = 85;
        damageChart[UnitType::Tank][UnitType::Tank] = 55;
        damageChart[UnitType::Tank][UnitType::MediumTank] = 15;
        damageChart[UnitType::Tank][UnitType::Neotank] = 1;
        damageChart[UnitType::Tank][UnitType::APC] = 75;
        damageChart[UnitType::Tank][UnitType::Artillery] = 70;
        damageChart[UnitType::Tank][UnitType::Rockets] = 85;
        damageChart[UnitType::Tank][UnitType::AntiAir] = 65;
        damageChart[UnitType::Tank][UnitType::Missiles] = 85;
        damageChart[UnitType::Tank][UnitType::PipeRunner] = 25;

        // Medium Tank vs targets
        damageChart[UnitType::MediumTank][UnitType::Infantry] = 105;
        damageChart[UnitType::MediumTank][UnitType::Mech] = 95;
        damageChart[UnitType::MediumTank][UnitType::Recon] = 105;
        damageChart[UnitType::MediumTank][UnitType::Tank] = 85;
        damageChart[UnitType::MediumTank][UnitType::MediumTank] = 55;
        damageChart[UnitType::MediumTank][UnitType::Neotank] = 15;
        damageChart[UnitType::MediumTank][UnitType::Megatank] = 1;
        damageChart[UnitType::MediumTank][UnitType::APC] = 105;
        damageChart[UnitType::MediumTank][UnitType::Artillery] = 105;
        damageChart[UnitType::MediumTank][UnitType::Rockets] = 105;
        damageChart[UnitType::MediumTank][UnitType::AntiAir] = 105;
        damageChart[UnitType::MediumTank][UnitType::Missiles] = 105;
        damageChart[UnitType::MediumTank][UnitType::PipeRunner] = 45;

        // Anti-Air vs air targets
        damageChart[UnitType::AntiAir][UnitType::Infantry] = 105;
        damageChart[UnitType::AntiAir][UnitType::Mech] = 105;
        damageChart[UnitType::AntiAir][UnitType::BattleCopter] = 120;
        damageChart[UnitType::AntiAir][UnitType::TransportCopter] = 120;
        damageChart[UnitType::AntiAir][UnitType::Fighter] = 65;
        damageChart[UnitType::AntiAir][UnitType::Bomber] = 75;

        // Artillery (indirect fire)
        damageChart[UnitType::Artillery][UnitType::Infantry] = 90;
        damageChart[UnitType::Artillery][UnitType::Mech] = 85;
        damageChart[UnitType::Artillery][UnitType::Recon] = 80;
        damageChart[UnitType::Artillery][UnitType::Tank] = 70;
        damageChart[UnitType::Artillery][UnitType::MediumTank] = 45;
        damageChart[UnitType::Artillery][UnitType::APC] = 70;
        damageChart[UnitType::Artillery][UnitType::Artillery] = 75;
        damageChart[UnitType::Artillery][UnitType::Rockets] = 80;
        damageChart[UnitType::Artillery][UnitType::AntiAir] = 70;

        // Rockets (indirect fire)
        damageChart[UnitType::Rockets][UnitType::Infantry] = 95;
        damageChart[UnitType::Rockets][UnitType::Mech] = 90;
        damageChart[UnitType::Rockets][UnitType::Recon] = 90;
        damageChart[UnitType::Rockets][UnitType::Tank] = 80;
        damageChart[UnitType::Rockets][UnitType::MediumTank] = 55;
        damageChart[UnitType::Rockets][UnitType::APC] = 80;
        damageChart[UnitType::Rockets][UnitType::Artillery] = 80;
        damageChart[UnitType::Rockets][UnitType::Rockets] = 85;
        damageChart[UnitType::Rockets][UnitType::AntiAir] = 85;

        // Battle Copter
        damageChart[UnitType::BattleCopter][UnitType::Infantry] = 75;
        damageChart[UnitType::BattleCopter][UnitType::Mech] = 75;
        damageChart[UnitType::BattleCopter][UnitType::Recon] = 55;
        damageChart[UnitType::BattleCopter][UnitType::Tank] = 55;
        damageChart[UnitType::BattleCopter][UnitType::MediumTank] = 25;
        damageChart[UnitType::BattleCopter][UnitType::APC] = 60;
        damageChart[UnitType::BattleCopter][UnitType::Artillery] = 65;
        damageChart[UnitType::BattleCopter][UnitType::AntiAir] = 25;
        damageChart[UnitType::BattleCopter][UnitType::BattleCopter] = 65;
        damageChart[UnitType::BattleCopter][UnitType::TransportCopter] = 95;

        // Fighter
        damageChart[UnitType::Fighter][UnitType::BattleCopter] = 100;
        damageChart[UnitType::Fighter][UnitType::TransportCopter] = 100;
        damageChart[UnitType::Fighter][UnitType::Fighter] = 55;
        damageChart[UnitType::Fighter][UnitType::Bomber] = 100;

        // Bomber
        damageChart[UnitType::Bomber][UnitType::Infantry] = 110;
        damageChart[UnitType::Bomber][UnitType::Mech] = 110;
        damageChart[UnitType::Bomber][UnitType::Recon] = 105;
        damageChart[UnitType::Bomber][UnitType::Tank] = 105;
        damageChart[UnitType::Bomber][UnitType::MediumTank] = 95;
        damageChart[UnitType::Bomber][UnitType::Neotank] = 90;
        damageChart[UnitType::Bomber][UnitType::Megatank] = 35;
        damageChart[UnitType::Bomber][UnitType::APC] = 105;
        damageChart[UnitType::Bomber][UnitType::Artillery] = 105;
        damageChart[UnitType::Bomber][UnitType::Rockets] = 105;
        damageChart[UnitType::Bomber][UnitType::AntiAir] = 95;
        damageChart[UnitType::Bomber][UnitType::Missiles] = 105;

        // Battleship
        damageChart[UnitType::Battleship][UnitType::Recon] = 90;
        damageChart[UnitType::Battleship][UnitType::Tank] = 80;
        damageChart[UnitType::Battleship][UnitType::MediumTank] = 55;
        damageChart[UnitType::Battleship][UnitType::APC] = 80;
        damageChart[UnitType::Battleship][UnitType::Artillery] = 80;
        damageChart[UnitType::Battleship][UnitType::Rockets] = 85;
        damageChart[UnitType::Battleship][UnitType::Cruiser] = 95;
        damageChart[UnitType::Battleship][UnitType::Submarine] = 95;
        damageChart[UnitType::Battleship][UnitType::Battleship] = 50;
        damageChart[UnitType::Battleship][UnitType::Lander] = 95;

        // Cruiser
        damageChart[UnitType::Cruiser][UnitType::BattleCopter] = 115;
        damageChart[UnitType::Cruiser][UnitType::TransportCopter] = 115;
        damageChart[UnitType::Cruiser][UnitType::Fighter] = 55;
        damageChart[UnitType::Cruiser][UnitType::Bomber] = 65;
        damageChart[UnitType::Cruiser][UnitType::Submarine] = 90;

        // Submarine
        damageChart[UnitType::Submarine][UnitType::Battleship] = 55;
        damageChart[UnitType::Submarine][UnitType::Cruiser] = 25;
        damageChart[UnitType::Submarine][UnitType::Submarine] = 55;
        damageChart[UnitType::Submarine][UnitType::Lander] = 95;
        damageChart[UnitType::Submarine][UnitType::BlackBoat] = 95;
        damageChart[UnitType::Submarine][UnitType::Carrier] = 75;

        // Units that can't attack (transports, etc.) are handled by returning
        // NO_DAMAGE
    }

    inline int
    DamageTable::getBaseDamage(UnitType attacker, UnitType defender) {
        initializeDamageChart();

        auto attackerIt = damageChart.find(attacker);
        if(attackerIt == damageChart.end()) {
            return NO_DAMAGE;
        }

        auto defenderIt = attackerIt->second.find(defender);
        if(defenderIt == attackerIt->second.end()) {
            return NO_DAMAGE;
        }

        return defenderIt->second;
    }

} // namespace OpenWars::Game