#pragma once

#include "../../core/vector.hpp"
#include "../../core/colors.hpp"
#include "../co/co.hpp"
#include "../map/terrain.hpp"
#include <string>

namespace OpenWars::Game {
    enum class UnitType {
        // Infantry
        Infantry,
        Mech,

        // Direct Combat Vehicles
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

        // Copters
        BattleCopter,
        TransportCopter,

        // Planes
        Fighter,
        Bomber,
        StealthBomber,
        BlackBomb,

        // Ships
        Battleship,
        Cruiser,
        Submarine,
        BlackBoat,
        Carrier,
        Lander
    };

    enum class MovementType {
        Infantry,
        Mech,
        Tire,
        Tread,
        Air,
        Sea,
        Lander,
        Pipe
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

    class Unit {
      protected:
        // Identity
        UnitType type;
        std::string name;
        int playerID;
        CO* commandingOfficer;

        // Position
        Vector2 gridPos;

        // Stats
        int maxHP;     // Always 10 in AW (displayed as 10, stored as 100)
        int currentHP; // 1-10 (stored as 10-100 internally)
        int maxFuel;
        int currentFuel;
        int maxAmmo; // -1 means infinite
        int currentAmmo;

        // Movement
        MovementType movementType;
        int moveRange;

        // Combat
        WeaponType primaryWeapon;
        WeaponType secondaryWeapon;
        int minAttackRange;
        int maxAttackRange;
        int vision;

        // Cost
        int deploymentCost;

        // State
        bool hasActed;
        bool isVisible;
        bool isHidden; // Submarines, stealth
        bool canCapture;

        // Visual
        Color teamColor;

      public:
        Unit(UnitType type, int playerID, Vector2 startPos);
        virtual ~Unit() = default;

        // Core actions
        virtual bool
        canMoveTo(const Vector2& target, const Terrain* terrain) const;
        virtual void moveTo(const Vector2& target);
        virtual bool canAttack(const Unit* target) const;
        virtual int calculateBaseDamage(const Unit* target) const = 0;
        virtual void attack(Unit* target, const Terrain* defenderTerrain);
        virtual void takeDamage(int damage);

        // Special abilities
        virtual bool canCounter(const Unit* attacker) const;
        virtual void capture(/* Building* building */);
        virtual void join(Unit* other); // Merge with same unit type
        virtual void hide();
        virtual void unhide();

        // Fuel & Ammo
        virtual void consumeFuel(int amount);
        virtual void consumeAmmo(int amount);
        virtual bool hasFuel() const {
            return currentFuel > 0;
        }
        virtual bool hasAmmo() const {
            return maxAmmo == -1 || currentAmmo > 0;
        }
        virtual void resupply();

        // State management
        virtual void beginTurn();
        virtual void endTurn();
        virtual bool isDestroyed() const {
            return currentHP <= 0;
        }

        // Rendering
        virtual void render(Vector2 screenPos) const = 0;

        // Getters
        UnitType getType() const {
            return type;
        }
        const std::string& getName() const {
            return name;
        }
        Vector2 getGridPos() const {
            return gridPos;
        }
        int getHP() const {
            return currentHP;
        }
        int getDisplayHP() const {
            return (currentHP + 9) / 10;
        } // Round up
        int getMaxHP() const {
            return maxHP;
        }
        int getMoveRange() const {
            return moveRange;
        }
        MovementType getMovementType() const {
            return movementType;
        }
        bool getHasActed() const {
            return hasActed;
        }
        int getPlayerID() const {
            return playerID;
        }
        int getVision() const {
            return vision;
        }
        int getCost() const {
            return deploymentCost;
        }
        bool getCanCapture() const {
            return canCapture;
        }
        CO* getCO() const {
            return commandingOfficer;
        }

        // Setters
        void setCO(CO* co) {
            commandingOfficer = co;
        }
        void setTeamColor(const Color& color) {
            teamColor = color;
        }

        // Distance calculation
        static int getManhattanDistance(const Vector2& a, const Vector2& b) {
            return std::abs(a.x - b.x) + std::abs(a.y - b.y);
        }
    };

} // namespace OpenWars::Game