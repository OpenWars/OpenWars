#pragma once

#include <string>
#include <vector>
#include "../movement.hpp"

namespace OpenWars::Game {

    struct UnitSprite {
        int row = 0, col = 0;
        int frameCount = 1;
        float animSpeed = 0.0f;
    };

    enum class WeaponType {
        None,
        MachineGun,
        Bazooka,
        Cannon,
        AntiAir,
        Artillery,
        Rockets,
        Missiles,
        Vulcan,
        Bombs,
        AirToAir,
        Torpedoes,
        AntiShip
    };

    struct UnitDefinition {
        std::string id;
        std::string name;
        MovementType movementType = MovementType::Infantry;
        int moveRange = 3;
        int vision = 2;
        int maxFuel = 99;
        int maxAmmo = -1;
        int deploymentCost = 1000;
        WeaponType primaryWeapon = WeaponType::None;
        WeaponType secondaryWeapon = WeaponType::None;
        int minAttackRange = 1;
        int maxAttackRange = 1;
        bool canCapture = false;
        bool canCounterAttack = true;
        int cargoCapacity = 0;
        std::vector<std::string> loadableTypes;
        bool canResupply = false;
        UnitSprite sprite;

        bool isIndirect() const {
            return minAttackRange > 1;
        }
        bool isTransport() const {
            return cargoCapacity > 0;
        }
    };

} // namespace OpenWars::Game