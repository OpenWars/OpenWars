#include "unit.hpp"
#include "damage.hpp"
#include <algorithm>

namespace OpenWars::Game {

    Unit::Unit(const UnitDefinition* def, int playerID, Vector2 startPos)
        : definition(def)
        , playerID(playerID)
        , commandingOfficer(nullptr)
        , gridPos(startPos)
        , currentFuel(def->maxFuel)
        , currentAmmo(def->maxAmmo > 0 ? def->maxAmmo : 0)
        , teamColor(Colors::RED_500) {
    }

    void Unit::moveTo(const Vector2& target) {
        if(!canMoveTo(target, nullptr))
            return;

        int distance = getManhattanDistance(gridPos, target);
        gridPos = target;
        consumeFuel(distance);
        hasActed = true;
    }

    bool Unit::canMoveTo(const Vector2& target, const Terrain* terrain) const {
        if(hasActed || !hasFuel())
            return false;

        int effectiveRange = definition->moveRange;
        if(commandingOfficer)
            effectiveRange += commandingOfficer->getMovementBonus();

        return getManhattanDistance(gridPos, target) <= effectiveRange;
    }

    bool Unit::canAttack(const Unit* target) const {
        if(!target || hasActed || target->playerID == playerID || !hasAmmo())
            return false;

        int dist = getManhattanDistance(gridPos, target->gridPos);
        return dist >= definition->minAttackRange &&
               dist <= definition->maxAttackRange;
    }

    int Unit::calculateBaseDamage(const Unit* target) const {
        return DamageTable::get().lookup(
            definition->id,
            target->definition->id
        );
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

        if(commandingOfficer)
            commandingOfficer->addStars(damage / 10);

        if(!target->isDestroyed() && target->canCounter(this)) {
            int counterDamage =
                DamageCalculator::calculateDamage(target, this, nullptr, true);
            takeDamage(counterDamage);

            if(target->commandingOfficer)
                target->commandingOfficer->addStars(counterDamage / 10);
        }

        if(definition->maxAmmo > 0)
            consumeAmmo(1);

        hasActed = true;
    }

    void Unit::takeDamage(int damage) {
        currentHP -= damage;
        if(currentHP < 0)
            currentHP = 0;
    }

    bool Unit::canCounter(const Unit* attacker) const {
        if(isDestroyed() || !hasAmmo() || !definition->canCounterAttack)
            return false;

        int dist = getManhattanDistance(gridPos, attacker->gridPos);
        return dist >= definition->minAttackRange &&
               dist <= definition->maxAttackRange;
    }

    void Unit::capture() {
        if(!definition->canCapture)
            return;
    }

    void Unit::join(Unit* other) {
        if(!other || other->definition->id != definition->id ||
           other->playerID != playerID)
            return;

        currentHP = std::min(100, currentHP + other->currentHP);
        currentAmmo = std::max(currentAmmo, other->currentAmmo);
        currentFuel = std::max(currentFuel, other->currentFuel);
    }

    void Unit::hide() {
        hidden = true;
        unitVisible = false;
    }

    void Unit::unhide() {
        hidden = false;
        unitVisible = true;
    }

    void Unit::consumeFuel(int amount) {
        currentFuel -= amount;
        if(currentFuel < 0) {
            currentFuel = 0;
            takeDamage(10);
        }
    }

    void Unit::consumeAmmo(int amount) {
        if(definition->maxAmmo == -1)
            return;

        currentAmmo -= amount;
        if(currentAmmo < 0)
            currentAmmo = 0;
    }

    void Unit::resupply() {
        currentFuel = definition->maxFuel;
        if(definition->maxAmmo > 0)
            currentAmmo = definition->maxAmmo;
    }

    void Unit::beginTurn() {
        hasActed = false;

        if(definition->movementType == MovementType::Air)
            consumeFuel(2);

        if(commandingOfficer)
            commandingOfficer->applyUnitModifiers(this);
    }

    void Unit::endTurn() {
    }

    bool Unit::canLoad(const Unit* unit) const {
        if(!unit || !hasCargoSpace())
            return false;

        const auto& loadable = definition->loadableTypes;
        return std::find(
                   loadable.begin(),
                   loadable.end(),
                   unit->definition->id
               ) != loadable.end();
    }

    void Unit::load(std::shared_ptr<Unit> unit) {
        if(!canLoad(unit.get()))
            return;
        cargo.push_back(std::move(unit));
    }

    std::shared_ptr<Unit> Unit::unload(int index) {
        if(index < 0 || index >= (int)cargo.size())
            return nullptr;

        auto unit = cargo[index];
        cargo.erase(cargo.begin() + index);
        return unit;
    }

} // namespace OpenWars::Game