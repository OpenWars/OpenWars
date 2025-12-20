#pragma once

#include <memory>
#include <string>
#include <functional>
#include "../../core/colors.hpp"

namespace OpenWars::Game {

    // Forward declarations
    class Unit;
    class Terrain;

    enum class COPowerType {
        None,
        Normal, // CO Power
        Super   // Super CO Power
    };

    // CO stats modify unit performance
    struct COStats {
        int firepower = 100; // Attack modifier (100 = normal, 110 = +10%)
        int defense = 100;   // Defense modifier
        int movement = 0;    // Movement bonus (+1, +2, etc.)
        int luck = 0;        // Additional random damage (0-luck)

        // Special modifiers
        int terrainStars = 0;       // Additional defense stars
        int counterModifier = 100;  // Counter-attack modifier
        bool perfectVision = false; // Ignore fog of war
    };

    class CO {
      protected:
        std::string name;
        std::string country; // Orange Star, Blue Moon, etc.
        Color themeColor;

        // Base stats (always active)
        COStats baseStats;

        // CO Power stats
        COStats coPowerStats;
        int coPowerCost = 3; // Stars needed

        // Super CO Power stats
        COStats superPowerStats;
        int superPowerCost = 6; // Stars needed

        // Current state
        int currentStars = 0;
        int maxStars = 9;
        COPowerType activePower = COPowerType::None;
        int powerDuration = 0; // Turns remaining for power

        // Special abilities
        std::function<void(Unit*)> specialAbility;
        std::function<void()> coPowerAbility;
        std::function<void()> superPowerAbility;

      public:
        CO(const std::string& name, const std::string& country);
        virtual ~CO() = default;

        // Power management
        void addStars(int stars);
        bool canActivateCOPower() const;
        bool canActivateSuperPower() const;
        void activateCOPower();
        void activateSuperPower();
        void endTurn(); // Decrements power duration
        void deactivatePower();

        // Get current effective stats
        COStats getCurrentStats() const;

        // Apply CO effects to unit
        virtual void applyUnitModifiers(Unit* unit) const;

        // Calculate effective attack/defense for a unit
        int getAttackModifier() const;
        int getDefenseModifier() const;
        int getMovementBonus() const;
        int getLuck() const;

        // Getters
        const std::string& getName() const {
            return name;
        }
        const std::string& getCountry() const {
            return country;
        }
        Color getThemeColor() const {
            return themeColor;
        }
        int getCurrentStars() const {
            return currentStars;
        }
        int getMaxStars() const {
            return maxStars;
        }
        COPowerType getActivePower() const {
            return activePower;
        }
        bool isPowerActive() const {
            return activePower != COPowerType::None;
        }

        // CO Power descriptions (for UI)
        virtual std::string getCOPowerName() const = 0;
        virtual std::string getCOPowerDescription() const = 0;
        virtual std::string getSuperPowerName() const = 0;
        virtual std::string getSuperPowerDescription() const = 0;

      protected:
        // Helper to calculate stars gained from damage
        static int calculateStarsFromDamage(int damageDealt, int damageTaken);
    };

    class Andrea : public CO {
      public:
        Andrea();

        std::string getCOPowerName() const override {
            return "Hyper Repair";
        }
        std::string getCOPowerDescription() const override {
            return "Restores 2 HP to all units.";
        }
        std::string getSuperPowerName() const override {
            return "Hyper Upgrade";
        }
        std::string getSuperPowerDescription() const override {
            return "Restores 5 HP to all units. Firepower +10%.";
        }

        void applyUnitModifiers(Unit* unit) const override;
    };

    class Alexa : public CO {
      public:
        Alexa();

        std::string getCOPowerName() const override {
            return "Alexa Force";
        }
        std::string getCOPowerDescription() const override {
            return "Direct combat units gain +50% firepower.";
        }
        std::string getSuperPowerName() const override {
            return "Alexa Blast";
        }
        std::string getSuperPowerDescription() const override {
            return "Direct combat units gain +80% firepower, +1 movement.";
        }

        void applyUnitModifiers(Unit* unit) const override;
    };

    class Sam : public CO {
      public:
        Sam();

        std::string getCOPowerName() const override {
            return "Double Time";
        }
        std::string getCOPowerDescription() const override {
            return "Infantry gain +1 movement and capture speed doubled.";
        }
        std::string getSuperPowerName() const override {
            return "Victory March";
        }
        std::string getSuperPowerDescription() const override {
            return "Infantry gain +2 movement. Capture in 1 turn.";
        }

        void applyUnitModifiers(Unit* unit) const override;
    };

    class Gwen : public CO {
      public:
        Gwen();

        std::string getCOPowerName() const override {
            return "Snipe Attack";
        }
        std::string getCOPowerDescription() const override {
            return "Indirect units gain +1 range and +50% firepower.";
        }
        std::string getSuperPowerName() const override {
            return "Super Snipe";
        }
        std::string getSuperPowerDescription() const override {
            return "Indirect units gain +2 range and +80% firepower.";
        }

        void applyUnitModifiers(Unit* unit) const override;
    };

    class Claire : public CO {
      public:
        Claire();

        std::string getCOPowerName() const override {
            return "Gold Rush";
        }
        std::string getCOPowerDescription() const override {
            return "Gain funds equal to 50% of unit value.";
        }
        std::string getSuperPowerName() const override {
            return "Power of Money";
        }
        std::string getSuperPowerDescription() const override {
            return "Firepower increases with funds. Maximum +80%.";
        }

        void applyUnitModifiers(Unit* unit) const override;
    };

    class Evelyn : public CO {
      public:
        Evelyn();

        std::string getCOPowerName() const override {
            return "Lightning Drive";
        }
        std::string getCOPowerDescription() const override {
            return "All units can move again (non-infantry only).";
        }
        std::string getSuperPowerName() const override {
            return "Lightning Strike";
        }
        std::string getSuperPowerDescription() const override {
            return "All units can move again with +15% firepower.";
        }

        void applyUnitModifiers(Unit* unit) const override;
    };

    class Diana : public CO {
      public:
        Diana();

        std::string getCOPowerName() const override {
            return "Tsunami";
        }
        std::string getCOPowerDescription() const override {
            return "Naval units gain +50% firepower. Damages all enemies.";
        }
        std::string getSuperPowerName() const override {
            return "Typhoon";
        }
        std::string getSuperPowerDescription() const override {
            return "Naval units gain +80% firepower. Heavy damage to all "
                   "enemies.";
        }

        void applyUnitModifiers(Unit* unit) const override;
    };

    class Kanako : public CO {
      public:
        Kanako();

        std::string getCOPowerName() const override {
            return "Morale Boost";
        }
        std::string getCOPowerDescription() const override {
            return "+30% attack and defense to all units.";
        }
        std::string getSuperPowerName() const override {
            return "Samurai Spirit";
        }
        std::string getSuperPowerDescription() const override {
            return "+50% attack and defense. Counter-attacks deal full damage.";
        }

        void applyUnitModifiers(Unit* unit) const override;
    };

    class Shinji : public CO {
      public:
        Shinji();

        std::string getCOPowerName() const override {
            return "Enhanced Vision";
        }
        std::string getCOPowerDescription() const override {
            return "Perfect vision. Enemies suffer -30% defense.";
        }
        std::string getSuperPowerName() const override {
            return "Counter Break";
        }
        std::string getSuperPowerDescription() const override {
            return "Perfect vision. Enemies suffer -50% defense and cannot "
                   "counter.";
        }

        void applyUnitModifiers(Unit* unit) const override;
    };

    // Factory function
    std::unique_ptr<CO> createCO(const std::string& coName);

} // namespace OpenWars::Game