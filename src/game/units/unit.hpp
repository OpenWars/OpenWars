#pragma once

#include "../../core/vector.hpp"
#include "../../core/colors.hpp"
#include "definition.hpp"
#include "../co/co.hpp"
#include <string>
#include <vector>
#include <memory>

namespace OpenWars::Game {

    class Terrain;

    class Unit {
        protected:
        const UnitDefinition* definition;
        int playerID;
        CO* commandingOfficer;
        Vector2 gridPos;

        int currentHP = 100;
        int currentFuel;
        int currentAmmo;

        bool hasActed = false;
        bool unitVisible = true;
        bool hidden = false;

        Color teamColor;

        std::vector<std::shared_ptr<Unit>> cargo;

        public:
        Unit(const UnitDefinition* def, int playerID, Vector2 startPos);
        virtual ~Unit() = default;

        bool canMoveTo(const Vector2& target, const Terrain* terrain) const;
        void moveTo(const Vector2& target);
        bool canAttack(const Unit* target) const;
        int calculateBaseDamage(const Unit* target) const;
        void attack(Unit* target, const Terrain* defenderTerrain);
        void takeDamage(int damage);

        bool canCounter(const Unit* attacker) const;
        void capture();
        void join(Unit* other);
        void hide();
        void unhide();
        void consumeFuel(int amount);
        void consumeAmmo(int amount);
        void resupply();
        void beginTurn();
        void endTurn();

        bool canLoad(const Unit* unit) const;
        void load(std::shared_ptr<Unit> unit);
        std::shared_ptr<Unit> unload(int index);

        bool hasFuel() const {
            return currentFuel > 0;
        }
        bool hasAmmo() const {
            return definition->maxAmmo == -1 || currentAmmo > 0;
        }
        bool isDestroyed() const {
            return currentHP <= 0;
        }

        const std::string& getTypeId() const {
            return definition->id;
        }
        const std::string& getName() const {
            return definition->name;
        }
        const UnitDefinition& getDef() const {
            return *definition;
        }
        Vector2 getGridPos() const {
            return gridPos;
        }
        int getHP() const {
            return currentHP;
        }
        int getDisplayHP() const {
            return (currentHP + 9) / 10;
        }
        int getMoveRange() const {
            return definition->moveRange;
        }
        MovementType getMovementType() const {
            return definition->movementType;
        }
        bool getHasActed() const {
            return hasActed;
        }
        int getPlayerID() const {
            return playerID;
        }
        int getVision() const {
            return definition->vision;
        }
        int getCost() const {
            return definition->deploymentCost;
        }
        bool getCanCapture() const {
            return definition->canCapture;
        }
        CO* getCO() const {
            return commandingOfficer;
        }
        bool isCargoEmpty() const {
            return cargo.empty();
        }
        bool hasCargoSpace() const {
            return (int)cargo.size() < definition->cargoCapacity;
        }
        const std::vector<std::shared_ptr<Unit>>& getCargo() const {
            return cargo;
        }

        void setCO(CO* co) {
            commandingOfficer = co;
        }
        void setTeamColor(const Color& color) {
            teamColor = color;
        }

        static int getManhattanDistance(const Vector2& a, const Vector2& b) {
            return (int)(std::abs(a.x - b.x) + std::abs(a.y - b.y));
        }
    };

} // namespace OpenWars::Game