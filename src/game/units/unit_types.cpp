#include "unit_types.hpp"
#include "damage.hpp"

namespace OpenWars::Game {

    // INFANTRY

    Infantry::Infantry(int playerID, Vector2 startPos)
        : Unit(UnitType::Infantry, playerID, startPos) {
        name = "Infantry";
        movementType = MovementType::Infantry;
        moveRange = 3;
        vision = 2;
        maxFuel = 99;
        currentFuel = 99;
        maxAmmo = -1; // Infinite ammo
        deploymentCost = 1000;
        primaryWeapon = WeaponType::MachineGun;
        minAttackRange = 1;
        maxAttackRange = 1;
        canCapture = true;
    }

    int Infantry::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    void Infantry::render(Vector2 screenPos) const {
        // TODO: Render infantry sprite
    }

    Mech::Mech(int playerID, Vector2 startPos)
        : Unit(UnitType::Mech, playerID, startPos) {
        name = "Mech";
        movementType = MovementType::Mech;
        moveRange = 2;
        vision = 2;
        maxFuel = 70;
        currentFuel = 70;
        maxAmmo = 3;
        currentAmmo = 3;
        deploymentCost = 3000;
        primaryWeapon = WeaponType::Bazooka;
        secondaryWeapon = WeaponType::MachineGun;
        minAttackRange = 1;
        maxAttackRange = 1;
        canCapture = true;
    }

    int Mech::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    void Mech::render(Vector2 screenPos) const {
        // TODO: Render mech sprite
    }

    // VEHICLES

    Recon::Recon(int playerID, Vector2 startPos)
        : Unit(UnitType::Recon, playerID, startPos) {
        name = "Recon";
        movementType = MovementType::TireA;
        moveRange = 8;
        vision = 5;
        maxFuel = 80;
        currentFuel = 80;
        maxAmmo = -1;
        deploymentCost = 4000;
        primaryWeapon = WeaponType::MachineGun;
        minAttackRange = 1;
        maxAttackRange = 1;
    }

    int Recon::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    void Recon::render(Vector2 screenPos) const {
        // TODO: Render recon sprite
    }

    Tank::Tank(int playerID, Vector2 startPos)
        : Unit(UnitType::Tank, playerID, startPos) {
        name = "Tank";
        movementType = MovementType::Tread;
        moveRange = 6;
        vision = 3;
        maxFuel = 70;
        currentFuel = 70;
        maxAmmo = 9;
        currentAmmo = 9;
        deploymentCost = 7000;
        primaryWeapon = WeaponType::Cannon;
        secondaryWeapon = WeaponType::MachineGun;
        minAttackRange = 1;
        maxAttackRange = 1;
    }

    int Tank::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    void Tank::render(Vector2 screenPos) const {
        // TODO: Render tank sprite
    }

    MediumTank::MediumTank(int playerID, Vector2 startPos)
        : Unit(UnitType::MediumTank, playerID, startPos) {
        name = "Md Tank";
        movementType = MovementType::Tread;
        moveRange = 5;
        vision = 1;
        maxFuel = 50;
        currentFuel = 50;
        maxAmmo = 8;
        currentAmmo = 8;
        deploymentCost = 16000;
        primaryWeapon = WeaponType::Cannon;
        secondaryWeapon = WeaponType::MachineGun;
        minAttackRange = 1;
        maxAttackRange = 1;
    }

    int MediumTank::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    void MediumTank::render(Vector2 screenPos) const {
        // TODO: Render medium tank sprite
    }

    AntiAir::AntiAir(int playerID, Vector2 startPos)
        : Unit(UnitType::AntiAir, playerID, startPos) {
        name = "Anti-Air";
        movementType = MovementType::Tread;
        moveRange = 6;
        vision = 2;
        maxFuel = 60;
        currentFuel = 60;
        maxAmmo = 9;
        currentAmmo = 9;
        deploymentCost = 8000;
        primaryWeapon = WeaponType::Vulcan;
        minAttackRange = 1;
        maxAttackRange = 1;
    }

    int AntiAir::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    void AntiAir::render(Vector2 screenPos) const {
        // TODO: Render anti-air sprite
    }

    // INDIRECT

    Artillery::Artillery(int playerID, Vector2 startPos)
        : Unit(UnitType::Artillery, playerID, startPos) {
        name = "Artillery";
        movementType = MovementType::Tread;
        moveRange = 5;
        vision = 1;
        maxFuel = 50;
        currentFuel = 50;
        maxAmmo = 9;
        currentAmmo = 9;
        deploymentCost = 6000;
        primaryWeapon = WeaponType::Artillery;
        minAttackRange = 2;
        maxAttackRange = 3;
    }

    int Artillery::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    bool Artillery::canCounter(const Unit* attacker) const {
        // Indirect units can't counter-attack
        return false;
    }

    void Artillery::render(Vector2 screenPos) const {
        // TODO: Render artillery sprite
    }

    Rockets::Rockets(int playerID, Vector2 startPos)
        : Unit(UnitType::Rockets, playerID, startPos) {
        name = "Rockets";
        movementType = MovementType::TireA;
        moveRange = 5;
        vision = 1;
        maxFuel = 50;
        currentFuel = 50;
        maxAmmo = 6;
        currentAmmo = 6;
        deploymentCost = 15000;
        primaryWeapon = WeaponType::Rockets;
        minAttackRange = 3;
        maxAttackRange = 5;
    }

    int Rockets::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    bool Rockets::canCounter(const Unit* attacker) const {
        return false;
    }

    void Rockets::render(Vector2 screenPos) const {
        // TODO: Render rockets sprite
    }

    // TRANSPORT

    TransportUnit::TransportUnit(
        UnitType type,
        int playerID,
        Vector2 startPos,
        int capacity
    )
        : Unit(type, playerID, startPos)
        , cargoCapacity(capacity) {
    }

    void TransportUnit::load(std::shared_ptr<Unit> unit) {
        if(!canLoad(unit.get()))
            return;
        if(cargo.size() >= (size_t)cargoCapacity)
            return;

        cargo.push_back(unit);
    }

    std::shared_ptr<Unit> TransportUnit::unload(int index) {
        if(index < 0 || index >= (int)cargo.size()) {
            return nullptr;
        }

        auto unit = cargo[index];
        cargo.erase(cargo.begin() + index);
        return unit;
    }

    APC::APC(int playerID, Vector2 startPos)
        : TransportUnit(UnitType::APC, playerID, startPos, 1) {
        name = "APC";
        movementType = MovementType::Tread;
        moveRange = 6;
        vision = 1;
        maxFuel = 70;
        currentFuel = 70;
        maxAmmo = -1;
        deploymentCost = 5000;
        primaryWeapon = WeaponType::None;
    }

    int APC::calculateBaseDamage(const Unit* target) const {
        return DamageTable::NO_DAMAGE; // APC can't attack
    }

    bool APC::canLoad(const Unit* unit) const {
        // Can only load infantry and mechs
        return unit->getType() == UnitType::Infantry ||
               unit->getType() == UnitType::Mech;
    }

    void APC::render(Vector2 screenPos) const {
        // TODO: Render APC sprite
    }

    void APC::resupply() {
        Unit::resupply();
        // TODO: Implement adjacent unit resupply
    }

    // AIR UNITS

    BattleCopter::BattleCopter(int playerID, Vector2 startPos)
        : Unit(UnitType::BattleCopter, playerID, startPos) {
        name = "B Copter";
        movementType = MovementType::Air;
        moveRange = 6;
        vision = 3;
        maxFuel = 99;
        currentFuel = 99;
        maxAmmo = 6;
        currentAmmo = 6;
        deploymentCost = 9000;
        primaryWeapon = WeaponType::AirToAir;
        secondaryWeapon = WeaponType::MachineGun;
        minAttackRange = 1;
        maxAttackRange = 1;
    }

    int BattleCopter::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    void BattleCopter::render(Vector2 screenPos) const {
        // TODO: Render battle copter sprite
    }

    Fighter::Fighter(int playerID, Vector2 startPos)
        : Unit(UnitType::Fighter, playerID, startPos) {
        name = "Fighter";
        movementType = MovementType::Air;
        moveRange = 9;
        vision = 2;
        maxFuel = 99;
        currentFuel = 99;
        maxAmmo = 9;
        currentAmmo = 9;
        deploymentCost = 20000;
        primaryWeapon = WeaponType::AirToAir;
        minAttackRange = 1;
        maxAttackRange = 1;
    }

    int Fighter::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    void Fighter::render(Vector2 screenPos) const {
        // TODO: Render fighter sprite
    }

    Bomber::Bomber(int playerID, Vector2 startPos)
        : Unit(UnitType::Bomber, playerID, startPos) {
        name = "Bomber";
        movementType = MovementType::Air;
        moveRange = 7;
        vision = 2;
        maxFuel = 99;
        currentFuel = 99;
        maxAmmo = 9;
        currentAmmo = 9;
        deploymentCost = 22000;
        primaryWeapon = WeaponType::Bombs;
        minAttackRange = 1;
        maxAttackRange = 1;
    }

    int Bomber::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    void Bomber::render(Vector2 screenPos) const {
        // TODO: Render bomber sprite
    }

    // NAVAL

    Battleship::Battleship(int playerID, Vector2 startPos)
        : Unit(UnitType::Battleship, playerID, startPos) {
        name = "Battleship";
        movementType = MovementType::Sea;
        moveRange = 5;
        vision = 2;
        maxFuel = 99;
        currentFuel = 99;
        maxAmmo = 9;
        currentAmmo = 9;
        deploymentCost = 28000;
        primaryWeapon = WeaponType::AntiShip;
        minAttackRange = 2;
        maxAttackRange = 6;
    }

    int Battleship::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    void Battleship::render(Vector2 screenPos) const {
        // TODO: Render battleship sprite
    }

    Cruiser::Cruiser(int playerID, Vector2 startPos)
        : TransportUnit(UnitType::Cruiser, playerID, startPos, 2) {
        name = "Cruiser";
        movementType = MovementType::Sea;
        moveRange = 6;
        vision = 3;
        maxFuel = 99;
        currentFuel = 99;
        maxAmmo = 9;
        currentAmmo = 9;
        deploymentCost = 18000;
        primaryWeapon = WeaponType::AntiAir;
        secondaryWeapon = WeaponType::Torpedoes;
        minAttackRange = 1;
        maxAttackRange = 1;
    }

    int Cruiser::calculateBaseDamage(const Unit* target) const {
        return DamageTable::getBaseDamage(type, target->getType());
    }

    bool Cruiser::canLoad(const Unit* unit) const {
        // Can load copters only
        return unit->getType() == UnitType::BattleCopter ||
               unit->getType() == UnitType::TransportCopter;
    }

    void Cruiser::render(Vector2 screenPos) const {
        // TODO: Render cruiser sprite
    }

    // FACTORY
    std::unique_ptr<Unit>
    createUnit(UnitType type, int playerID, Vector2 startPos) {
        switch(type) {
        case UnitType::Infantry:
            return std::make_unique<Infantry>(playerID, startPos);
        case UnitType::Mech:
            return std::make_unique<Mech>(playerID, startPos);
        case UnitType::Recon:
            return std::make_unique<Recon>(playerID, startPos);
        case UnitType::Tank:
            return std::make_unique<Tank>(playerID, startPos);
        case UnitType::MediumTank:
            return std::make_unique<MediumTank>(playerID, startPos);
        case UnitType::AntiAir:
            return std::make_unique<AntiAir>(playerID, startPos);
        case UnitType::Artillery:
            return std::make_unique<Artillery>(playerID, startPos);
        case UnitType::Rockets:
            return std::make_unique<Rockets>(playerID, startPos);
        case UnitType::APC:
            return std::make_unique<APC>(playerID, startPos);
        case UnitType::BattleCopter:
            return std::make_unique<BattleCopter>(playerID, startPos);
        case UnitType::Fighter:
            return std::make_unique<Fighter>(playerID, startPos);
        case UnitType::Bomber:
            return std::make_unique<Bomber>(playerID, startPos);
        case UnitType::Battleship:
            return std::make_unique<Battleship>(playerID, startPos);
        case UnitType::Cruiser:
            return std::make_unique<Cruiser>(playerID, startPos);
        default:
            return nullptr;
        }
    }

} // namespace OpenWars::Game