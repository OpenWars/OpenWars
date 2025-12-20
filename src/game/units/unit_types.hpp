#pragma once

#include "unit.hpp"
#include <memory>
#include <vector>

namespace OpenWars::Game {
    class Infantry : public Unit {
      public:
        Infantry(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class Mech : public Unit {
      public:
        Mech(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class Recon : public Unit {
      public:
        Recon(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class Tank : public Unit {
      public:
        Tank(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class MediumTank : public Unit {
      public:
        MediumTank(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class Neotank : public Unit {
      public:
        Neotank(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class Megatank : public Unit {
      public:
        Megatank(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class AntiAir : public Unit {
      public:
        AntiAir(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class Artillery : public Unit {
      public:
        Artillery(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        bool canCounter(const Unit* attacker) const override;
        void render(Vector2 screenPos) const override;
    };

    class Rockets : public Unit {
      public:
        Rockets(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        bool canCounter(const Unit* attacker) const override;
        void render(Vector2 screenPos) const override;
    };

    class Missiles : public Unit {
      public:
        Missiles(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        bool canCounter(const Unit* attacker) const override;
        void render(Vector2 screenPos) const override;
    };

    class PipeRunner : public Unit {
      public:
        PipeRunner(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class TransportUnit : public Unit {
      protected:
        std::vector<std::shared_ptr<Unit>> cargo;
        int cargoCapacity;

      public:
        TransportUnit(
            UnitType type,
            int playerID,
            Vector2 startPos,
            int capacity
        );

        virtual bool canLoad(const Unit* unit) const = 0;
        virtual void load(std::shared_ptr<Unit> unit);
        virtual std::shared_ptr<Unit> unload(int index);

        const std::vector<std::shared_ptr<Unit>>& getCargo() const {
            return cargo;
        }
        bool hasSpace() const {
            return cargo.size() < (size_t)cargoCapacity;
        }
        bool isEmpty() const {
            return cargo.empty();
        }
    };

    class APC : public TransportUnit {
      public:
        APC(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        bool canLoad(const Unit* unit) const override;
        void render(Vector2 screenPos) const override;
        void resupply() override; // APCs can resupply adjacent units
    };

    class TransportCopter : public TransportUnit {
      public:
        TransportCopter(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        bool canLoad(const Unit* unit) const override;
        void render(Vector2 screenPos) const override;
    };

    class Lander : public TransportUnit {
      public:
        Lander(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        bool canLoad(const Unit* unit) const override;
        void render(Vector2 screenPos) const override;
    };

    class Cruiser : public TransportUnit {
      public:
        Cruiser(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        bool canLoad(const Unit* unit) const override;
        void render(Vector2 screenPos) const override;
    };

    class BlackBoat : public TransportUnit {
      public:
        BlackBoat(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        bool canLoad(const Unit* unit) const override;
        void render(Vector2 screenPos) const override;
        void resupply() override;
    };

    class Carrier : public TransportUnit {
      public:
        Carrier(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        bool canLoad(const Unit* unit) const override;
        void render(Vector2 screenPos) const override;
        void resupply() override;
    };

    class BattleCopter : public Unit {
      public:
        BattleCopter(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class Fighter : public Unit {
      public:
        Fighter(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class Bomber : public Unit {
      public:
        Bomber(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class StealthBomber : public Unit {
      public:
        StealthBomber(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class BlackBomb : public Unit {
      public:
        BlackBomb(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
        void explode(); // Special ability
    };

    class Battleship : public Unit {
      public:
        Battleship(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
    };

    class Submarine : public Unit {
      public:
        Submarine(int playerID, Vector2 startPos);
        int calculateBaseDamage(const Unit* target) const override;
        void render(Vector2 screenPos) const override;
        void dive();
        void surface();
    };

    std::unique_ptr<Unit>
    createUnit(UnitType type, int playerID, Vector2 startPos);

} // namespace OpenWars::Game