#pragma once

namespace OpenWars::Game {
    enum class UnitType {
        Infantry,
        Mech,
        Recon,
        Tank,
        MediumTank,
        Neotank,
        Megatank,
        APC,
        AntiAir,
        Artillery,
        Rockets,
        Missiles,
        PipeRunner,
        BattleCopter,
        TransportCopter,
        Fighter,
        Bomber,
        StealthBomber,
        BlackBomb,
        Battleship,
        Cruiser,
        Submarine,
        BlackBoat,
        Carrier,
        Lander
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
} // namespace OpenWars::Game