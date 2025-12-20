#include "unit.hpp"
#include "../co/co.hpp"
#include "damage.hpp"
#include <algorithm>

namespace OpenWars::Game {

    Unit::Unit(UnitType type, int playerID, Vector2 startPos)
        : type(type)
        , playerID(playerID)
        , gridPos(startPos)
        , commandingOfficer(nullptr)
        , maxHP(100)
        , currentHP(100)
        , maxFuel(99)
        , currentFuel(99)
        , maxAmmo(9)
        , currentAmmo(9)
        , movementType(MovementType::Infantry)
        , moveRange(3)
        , primaryWeapon(WeaponType::MachineGun)
        , secondaryWeapon(WeaponType::None)
        , minAttackRange(1)
        , maxAttackRange(1)
        , vision(2)
        , deploymentCost(1000)
        , hasActed(false)
        , isVisible(true)
        , isHidden(false)
        , canCapture(false)
        , teamColor(Colors::RED_500) {
    }

    void Unit::moveTo(const Vector2& target) {
        if(!canMoveTo(target, nullptr)) {
            return;
        }

        int distance = getManhattanDistance(gridPos, target);
        gridPos = target;

        // Consume fuel based on movement
        consumeFuel(distance);

        hasActed = true;
    }

    bool Unit::canMoveTo(const Vector2& target, const Terrain* terrain) const {
        if(hasActed)
            return false;
        if(!hasFuel())
            return false;

        int distance = getManhattanDistance(gridPos, target);

        // Basic movement range check
        int effectiveRange = moveRange;
        if(commandingOfficer) {
            effectiveRange += commandingOfficer->getMovementBonus();
        }

        return distance <= effectiveRange;
    }

    bool Unit::canAttack(const Unit* target) const {
        if(!target)
            return false;
        if(hasActed)
            return false;
        if(target->playerID == playerID)
            return false;
        if(!hasAmmo())
            return false;

        int distance = getManhattanDistance(gridPos, target->gridPos);
        return distance >= minAttackRange && distance <= maxAttackRange;
    }

    void Unit::attack(Unit* target, const Terrain* defenderTerrain) {
        if(!canAttack(target))
            return;

        int damage = DamageCalculator::calculateDamage(
            this,
            target,
            defenderTerrain,
            true
        );

        target->takeDamage(damage);

        // Add stars to CO based on damage dealt
        if(commandingOfficer) {
            int stars = damage / 10; // 1 star per 10 damage
            commandingOfficer->addStars(stars);
        }

        // Counter-attack if target can counter
        if(!target->isDestroyed() && target->canCounter(this)) {
            int counterDamage = DamageCalculator::calculateDamage(
                target,
                this,
                nullptr, // Attacker doesn't get terrain defense
                true
            );

            takeDamage(counterDamage);

            if(target->commandingOfficer) {
                int stars = counterDamage / 10;
                target->commandingOfficer->addStars(stars);
            }
        }

        // Consume ammo
        if(maxAmmo > 0) {
            consumeAmmo(1);
        }

        hasActed = true;
    }

    void Unit::takeDamage(int damage) {
        currentHP -= damage;
        if(currentHP < 0)
            currentHP = 0;
    }

    bool Unit::canCounter(const Unit* attacker) const {
        if(isDestroyed())
            return false;
        if(!hasAmmo())
            return false;

        // Check if we have a weapon that can hit the attacker
        int distance = getManhattanDistance(gridPos, attacker->gridPos);
        return distance >= minAttackRange && distance <= maxAttackRange;
    }

    void Unit::capture(/* Building* building */) {
        if(!canCapture)
            return;
        // Implementation would interact with building system
        // Capture progress = HP of capturing unit
    }

    void Unit::join(Unit* other) {
        if(!other)
            return;
        if(other->type != type)
            return;
        if(other->playerID != playerID)
            return;

        // Combine HP (max 100)
        int totalHP = currentHP + other->currentHP;
        currentHP = std::min(100, totalHP);

        // Combine ammo and fuel (take max of either)
        currentAmmo = std::max(currentAmmo, other->currentAmmo);
        currentFuel = std::max(currentFuel, other->currentFuel);

        // Other unit is consumed/destroyed
    }

    void Unit::hide() {
        isHidden = true;
        isVisible = false;
    }

    void Unit::unhide() {
        isHidden = false;
        isVisible = true;
    }

    void Unit::consumeFuel(int amount) {
        currentFuel -= amount;
        if(currentFuel < 0) {
            currentFuel = 0;
            // Unit takes damage from running out of fuel
            takeDamage(10); // 1 HP per turn without fuel
        }
    }

    void Unit::consumeAmmo(int amount) {
        if(maxAmmo == -1)
            return; // Infinite ammo

        currentAmmo -= amount;
        if(currentAmmo < 0)
            currentAmmo = 0;
    }

    void Unit::resupply() {
        currentFuel = maxFuel;
        currentAmmo = maxAmmo;
    }

    void Unit::beginTurn() {
        hasActed = false;

        // Air units consume fuel per turn
        if(movementType == MovementType::Air) {
            consumeFuel(2); // Most air units use 2 fuel per turn
        }

        // Apply CO modifiers
        if(commandingOfficer) {
            commandingOfficer->applyUnitModifiers(this);
        }
    }

    void Unit::endTurn() {
        // Reset any per-turn flags
    }

} // namespace OpenWars::Game