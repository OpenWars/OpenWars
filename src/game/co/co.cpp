#include "co.hpp"
#include "../units/unit.hpp"

namespace OpenWars::Game {

    CO::CO(const std::string& name, const std::string& country)
        : name(name)
        , country(country)
        , themeColor(Colors::BLUE_500) {
    }

    void CO::addStars(int stars) {
        currentStars += stars;
        if(currentStars > maxStars) {
            currentStars = maxStars;
        }
    }

    bool CO::canActivateCOPower() const {
        return currentStars >= coPowerCost && activePower == COPowerType::None;
    }

    bool CO::canActivateSuperPower() const {
        return currentStars >= superPowerCost &&
               activePower == COPowerType::None;
    }

    void CO::activateCOPower() {
        if(!canActivateCOPower())
            return;

        currentStars -= coPowerCost;
        activePower = COPowerType::Normal;
        powerDuration = 1; // Powers last 1 turn by default

        if(coPowerAbility) {
            coPowerAbility();
        }
    }

    void CO::activateSuperPower() {
        if(!canActivateSuperPower())
            return;

        currentStars -= superPowerCost;
        activePower = COPowerType::Super;
        powerDuration = 1;

        if(superPowerAbility) {
            superPowerAbility();
        }
    }

    void CO::endTurn() {
        if(powerDuration > 0) {
            powerDuration--;
            if(powerDuration <= 0) {
                deactivatePower();
            }
        }
    }

    void CO::deactivatePower() {
        activePower = COPowerType::None;
        powerDuration = 0;
    }

    COStats CO::getCurrentStats() const {
        switch(activePower) {
        case COPowerType::Normal:
            return coPowerStats;
        case COPowerType::Super:
            return superPowerStats;
        default:
            return baseStats;
        }
    }

    void CO::applyUnitModifiers(Unit* unit) const {
        // Default implementation - override in derived classes
    }

    int CO::getAttackModifier() const {
        return getCurrentStats().firepower;
    }

    int CO::getDefenseModifier() const {
        return getCurrentStats().defense;
    }

    int CO::getMovementBonus() const {
        return getCurrentStats().movement;
    }

    int CO::getLuck() const {
        return getCurrentStats().luck;
    }

    int CO::calculateStarsFromDamage(int damageDealt, int damageTaken) {
        // In AW, you gain stars based on damage dealt and taken
        // Roughly 1 star per 100 damage points
        int stars = (damageDealt + damageTaken) / 100;
        return stars;
    }

    Andrea::Andrea()
        : CO("Andrea", "Orange Star") {
        themeColor = Colors::ORANGE_500;

        // Andrea has no bonuses/penalties (100/100)
        baseStats.firepower = 100;
        baseStats.defense = 100;

        // CO Power: Hyper Repair (3 stars) - Restore 2 HP to all units, +10/10
        coPowerStats.firepower = 110;
        coPowerStats.defense = 110;
        coPowerCost = 3;

        coPowerAbility = [this]() {
            // Implementation would heal all units by 2 HP
        };

        // Super: Hyper Upgrade (6 stars) - Restore 5 HP, +10/10
        superPowerStats.firepower = 110;
        superPowerStats.defense = 110;
        superPowerCost = 6;

        superPowerAbility = [this]() {
            // Implementation would heal all units by 5 HP
        };
    }

    void Andrea::applyUnitModifiers(Unit* unit) const {
        // Andrea has no passive modifiers (100/100)
    }

    Alexa::Alexa()
        : CO("Alexa", "Orange Star") {
        themeColor = Colors::RED_500;

        // Alexa: 120/100, indirect 90/100
        baseStats.firepower = 120; // +20% for direct units
        baseStats.defense = 100;

        // CO Power: Alexa Force (3 stars) - 150/100, indirect 90/100
        coPowerStats.firepower = 150;
        coPowerStats.defense = 100;
        coPowerCost = 3;

        // Super: Alexa Blast (6 stars) - 180/100, indirect 90/100, +2 movement
        superPowerStats.firepower = 180;
        superPowerStats.defense = 100;
        superPowerStats.movement = 2;
        superPowerCost = 6;
    }

    void Alexa::applyUnitModifiers(Unit* unit) const {
        // Alexa's modifiers:
        // Direct units: 120/100 base, 150/100 COP, 180/100 SCOP
        // Indirect units: Always 90/100
        // SCOP also gives +2 movement to direct units
    }

    Sam::Sam()
        : CO("Sam", "Orange Star") {
        themeColor = Colors::YELLOW_500;

        // Sam: 100/100, transport +1 movement, direct +10% infantry
        baseStats.firepower = 100;
        baseStats.defense = 100;

        // CO Power: Double Time (3 stars) - 110/110, infantry +1 move, double
        // capture
        coPowerStats.firepower = 110;
        coPowerStats.defense = 110;
        coPowerStats.movement = 1; // Infantry only
        coPowerCost = 3;

        // Super: Victory March (9 stars) - 110/110, infantry +2 move, triple
        // capture
        superPowerStats.firepower = 110;
        superPowerStats.defense = 110;
        superPowerStats.movement = 2; // Infantry only
        superPowerCost = 9;
    }

    void Sam::applyUnitModifiers(Unit* unit) const {
        // Sam's modifiers:
        // Base: Infantry 110/100, transports +1 movement
        // COP: All 110/110, infantry +1 move, 2x capture speed
        // SCOP: All 110/110, infantry +2 move, 3x capture speed
    }

    Gwen::Gwen()
        : CO("Gwen", "Blue Moon") {
        themeColor = Colors::BLUE_400;

        // Gwen: 100/100, indirect 120/100 +1 range, direct 80/100
        baseStats.firepower = 100;
        baseStats.defense = 100;

        // CO Power: Snipe Attack (3 stars) - 130/100, indirect 150/100 +2
        // range, direct 80/100
        coPowerStats.firepower = 130;
        coPowerStats.defense = 100;
        coPowerCost = 3;

        // Super: Super Snipe (6 stars) - 130/100, indirect 180/100 +3 range,
        // direct 80/100
        superPowerStats.firepower = 130;
        superPowerStats.defense = 100;
        superPowerCost = 6;
    }

    void Gwen::applyUnitModifiers(Unit* unit) const {
        // Gwen's modifiers:
        // Base: Indirect 120/100 +1 range, Direct 80/100
        // COP: All 130/100, Indirect 150/100 +2 range (total +2), Direct 80/100
        // SCOP: All 130/100, Indirect 180/100 +3 range (total +3), Direct
        // 80/100
    }

    Claire::Claire()
        : CO("Claire", "Blue Moon") {
        themeColor = Colors::BLUE_300;

        // Claire: 90/100, units cost -20%, +50 luck
        baseStats.firepower = 90;
        baseStats.defense = 100;
        baseStats.luck = 50;

        // CO Power: Gold Rush (2 stars) - 110/100, gain funds, +50 luck
        coPowerStats.firepower = 110;
        coPowerStats.defense = 100;
        coPowerStats.luck = 50;
        coPowerCost = 2;

        // Super: Power of Money (5 stars) - 110/100, firepower increases with
        // funds (max 180/100)
        superPowerStats.firepower = 110; // Base, scales up to 180 with funds
        superPowerStats.defense = 100;
        superPowerStats.luck = 50;
        superPowerCost = 5;
    }

    void Claire::applyUnitModifiers(Unit* unit) const {
        // Claire's modifiers:
        // Base: 90/100, units cost 80%, +50 luck
        // COP: 110/100, gain funds equal to 50% of total unit value, +50 luck
        // SCOP: Firepower 110-180/100 based on funds (more money = more power),
        // +50 luck
    }

    Evelyn::Evelyn()
        : CO("Evelyn", "Green Earth") {
        themeColor = Colors::GREEN_500;

        // Evelyn: 100/100, air 115/100, naval 85/100
        baseStats.firepower = 100;
        baseStats.defense = 100;

        // CO Power: Lightning Drive (3 stars) - 110/100, air 125/100, naval
        // 85/100, non-infantry move again
        coPowerStats.firepower = 110;
        coPowerStats.defense = 100;
        coPowerCost = 3;

        // Super: Lightning Strike (6 stars) - 110/100, air 125/100, naval
        // 85/100, non-infantry move again
        superPowerStats.firepower = 110;
        superPowerStats.defense = 100;
        superPowerCost = 6;
    }

    void Evelyn::applyUnitModifiers(Unit* unit) const {
        // Evelyn's modifiers:
        // Base: Air 115/100, Naval 85/100, Others 100/100
        // COP: All 110/100, Air 125/100, Naval 85/100, non-infantry can move
        // again SCOP: All 110/100, Air 125/100, Naval 85/100, non-infantry can
        // move again
    }

    Diana::Diana()
        : CO("Diana", "Green Earth") {
        themeColor = Colors::TEAL_500;

        // Diana: 100/100, naval 120/100, air 90/100
        baseStats.firepower = 100;
        baseStats.defense = 100;

        // CO Power: Tsunami (4 stars) - 120/100, naval 150/100, air 90/100, 1
        // HP to all enemies
        coPowerStats.firepower = 120;
        coPowerStats.defense = 100;
        coPowerCost = 4;

        // Super: Typhoon (7 stars) - 120/100, naval 150/100, air 90/100, 2 HP
        // to all enemies
        superPowerStats.firepower = 120;
        superPowerStats.defense = 100;
        superPowerCost = 7;
    }

    void Diana::applyUnitModifiers(Unit* unit) const {
        // Diana's modifiers:
        // Base: Naval 120/100, Air 90/100, Others 100/100
        // COP: All 120/100, Naval 150/100, Air 90/100, damages all enemies for
        // 1 HP SCOP: All 120/100, Naval 150/100, Air 90/100, damages all
        // enemies for 2 HP
    }

    Kanako::Kanako()
        : CO("Kanako", "Yellow Comet") {
        themeColor = Colors::AMBER_600;

        // Kanako: 120/120, units cost +20%
        baseStats.firepower = 120;
        baseStats.defense = 120;

        // CO Power: Morale Boost (5 stars) - 130/130, counter 120/120
        coPowerStats.firepower = 130;
        coPowerStats.defense = 130;
        coPowerStats.counterModifier = 120;
        coPowerCost = 5;

        // Super: Samurai Spirit (9 stars) - 140/140, counter 140/140
        superPowerStats.firepower = 140;
        superPowerStats.defense = 140;
        superPowerStats.counterModifier = 140;
        superPowerCost = 9;
    }

    void Kanako::applyUnitModifiers(Unit* unit) const {
        // Kanako's modifiers:
        // Base: 120/120, units cost 120%
        // COP: 130/130, counter-attacks use 120/120
        // SCOP: 140/140, counter-attacks use 140/140
    }

    Shinji::Shinji()
        : CO("Shinji", "Yellow Comet") {
        themeColor = Colors::PURPLE_400;

        // Shinji: 100/100, +50% luck, +1 vision, hide HP, indirect -10%
        baseStats.firepower = 100;
        baseStats.defense = 100;
        baseStats.luck = 50;
        baseStats.perfectVision = true;

        // CO Power: Enhanced Vision (3 stars) - 110/110, +50 luck, enemy -10%
        // defense
        coPowerStats.firepower = 110;
        coPowerStats.defense = 110;
        coPowerStats.luck = 50;
        coPowerStats.perfectVision = true;
        coPowerCost = 3;

        // Super: Counter Break (6 stars) - 110/110, +50 luck, enemy -20%
        // defense
        superPowerStats.firepower = 110;
        superPowerStats.defense = 110;
        superPowerStats.luck = 50;
        superPowerStats.perfectVision = true;
        superPowerCost = 6;
    }

    void Shinji::applyUnitModifiers(Unit* unit) const {
        // Shinji's modifiers:
        // Base: 100/100, indirect 90/100, +1 vision, +50 luck, hides HP, bad
        // luck -0 COP: 110/110, +50 luck, enemy defense -10% SCOP: 110/110, +50
        // luck, enemy defense -20%
    }

    std::unique_ptr<CO> createCO(const std::string& coName) {
        if(coName == "Andrea")
            return std::make_unique<Andrea>();
        if(coName == "Alexa")
            return std::make_unique<Alexa>();
        if(coName == "Sam")
            return std::make_unique<Sam>();
        if(coName == "Gwen")
            return std::make_unique<Gwen>();
        if(coName == "Claire")
            return std::make_unique<Claire>();
        if(coName == "Evelyn")
            return std::make_unique<Evelyn>();
        if(coName == "Diana")
            return std::make_unique<Diana>();
        if(coName == "Kanako")
            return std::make_unique<Kanako>();
        if(coName == "Shinji")
            return std::make_unique<Shinji>();

        // Default to Andrea
        return std::make_unique<Andrea>();
    }

} // namespace OpenWars::Game