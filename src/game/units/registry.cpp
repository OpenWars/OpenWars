#include "registry.hpp"
#include "unit.hpp"
#include "damage.hpp"

namespace OpenWars::Game {

    UnitRegistry::UnitRegistry() {
        registerBuiltins(*this);
    }

    UnitRegistry& UnitRegistry::get() {
        static UnitRegistry instance;
        return instance;
    }

    void UnitRegistry::registerDefinition(UnitDefinition def) {
        definitions[def.id] = std::move(def);
    }

    const UnitDefinition* UnitRegistry::find(const std::string& id) const {
        auto it = definitions.find(id);
        return it != definitions.end() ? &it->second : nullptr;
    }

    std::unique_ptr<Unit> UnitRegistry::create(
        const std::string& id,
        int playerID,
        Vector2 startPos
    ) const {
        const UnitDefinition* def = find(id);
        if(!def)
            return nullptr;
        return std::make_unique<Unit>(def, playerID, startPos);
    }

    namespace {
        using MT = MovementType;
        using WT = WeaponType;

        UnitDefinition makeUnit(
            const std::string& id,
            const std::string& name,
            MT moveType,
            int moveRange,
            int vision,
            int maxFuel,
            int maxAmmo,
            int cost,
            WT primary = WT::None,
            WT secondary = WT::None,
            int minRange = 1,
            int maxRange = 1,
            bool capture = false,
            bool canCounter = true,
            int cargoCapacity = 0,
            std::vector<std::string> loadable = {},
            bool resupply = false
        ) {
            UnitDefinition d;
            d.id = id;
            d.name = name;
            d.movementType = moveType;
            d.moveRange = moveRange;
            d.vision = vision;
            d.maxFuel = maxFuel;
            d.maxAmmo = maxAmmo;
            d.deploymentCost = cost;
            d.primaryWeapon = primary;
            d.secondaryWeapon = secondary;
            d.minAttackRange = minRange;
            d.maxAttackRange = maxRange;
            d.canCapture = capture;
            d.canCounterAttack = canCounter;
            d.cargoCapacity = cargoCapacity;
            d.loadableTypes = std::move(loadable);
            d.canResupply = resupply;
            return d;
        }
    } // namespace

    void UnitRegistry::registerBuiltins(UnitRegistry& reg) {
        auto add = [&](UnitDefinition d) {
            reg.registerDefinition(std::move(d));
        };

        // all hail the unit registry cascade!
        add(makeUnit(
            "infantry",
            "Infantry",
            MT::Infantry,
            3,
            2,
            99,
            -1,
            1000,
            WT::MachineGun,
            WT::None,
            1,
            1,
            true,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "mech",
            "Mech",
            MT::Mech,
            2,
            2,
            70,
            3,
            3000,
            WT::Bazooka,
            WT::MachineGun,
            1,
            1,
            true,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "recon",
            "Recon",
            MT::Tire,
            8,
            5,
            80,
            -1,
            4000,
            WT::MachineGun,
            WT::None,
            1,
            1,
            false,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "tank",
            "Tank",
            MT::Tread,
            6,
            3,
            70,
            9,
            7000,
            WT::Cannon,
            WT::MachineGun,
            1,
            1,
            false,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "md_tank",
            "Md Tank",
            MT::Tread,
            5,
            1,
            50,
            8,
            16000,
            WT::Cannon,
            WT::MachineGun,
            1,
            1,
            false,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "neotank",
            "Neo Tank",
            MT::Tread,
            6,
            1,
            99,
            9,
            22000,
            WT::Cannon,
            WT::MachineGun,
            1,
            1,
            false,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "megatank",
            "Mega Tank",
            MT::Tread,
            4,
            1,
            50,
            3,
            28000,
            WT::Cannon,
            WT::MachineGun,
            1,
            1,
            false,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "anti_air",
            "Anti-Air",
            MT::Tread,
            6,
            2,
            60,
            9,
            8000,
            WT::Vulcan,
            WT::None,
            1,
            1,
            false,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "pipe_runner",
            "Pipe Runner",
            MT::Pipe,
            9,
            4,
            99,
            9,
            20000,
            WT::Cannon,
            WT::MachineGun,
            1,
            1,
            false,
            true,
            0,
            {}
        ));

        add(makeUnit(
            "artillery",
            "Artillery",
            MT::Tread,
            5,
            1,
            50,
            9,
            6000,
            WT::Artillery,
            WT::None,
            2,
            3,
            false,
            false,
            0,
            {}
        ));
        add(makeUnit(
            "rockets",
            "Rockets",
            MT::Tire,
            5,
            1,
            50,
            6,
            15000,
            WT::Rockets,
            WT::None,
            3,
            5,
            false,
            false,
            0,
            {}
        ));
        add(makeUnit(
            "missiles",
            "Missiles",
            MT::Tire,
            5,
            5,
            50,
            6,
            12000,
            WT::Missiles,
            WT::None,
            3,
            5,
            false,
            false,
            0,
            {}
        ));

        add(makeUnit(
            "apc",
            "APC",
            MT::Tread,
            6,
            1,
            70,
            -1,
            5000,
            WT::None,
            WT::None,
            1,
            1,
            false,
            false,
            1,
            {"infantry", "mech"},
            true
        ));
        add(makeUnit(
            "t_copter",
            "T Copter",
            MT::Air,
            6,
            2,
            99,
            -1,
            5000,
            WT::None,
            WT::None,
            1,
            1,
            false,
            false,
            1,
            {"infantry", "mech"}
        ));
        add(makeUnit(
            "lander",
            "Lander",
            MT::Lander,
            6,
            1,
            99,
            -1,
            12000,
            WT::None,
            WT::None,
            1,
            1,
            false,
            false,
            2,
            {"recon",
             "tank",
             "md_tank",
             "neotank",
             "megatank",
             "anti_air",
             "artillery",
             "rockets",
             "missiles",
             "apc",
             "infantry",
             "mech",
             "pipe_runner"}
        ));
        add(makeUnit(
            "black_boat",
            "Black Boat",
            MT::Sea,
            7,
            1,
            60,
            -1,
            7500,
            WT::None,
            WT::None,
            1,
            1,
            false,
            false,
            2,
            {"infantry", "mech"},
            true
        ));
        add(makeUnit(
            "carrier",
            "Carrier",
            MT::Sea,
            5,
            4,
            99,
            -1,
            30000,
            WT::None,
            WT::None,
            1,
            1,
            false,
            false,
            2,
            {"b_copter",
             "t_copter",
             "fighter",
             "bomber",
             "stealth",
             "black_bomb"},
            true
        ));

        add(makeUnit(
            "b_copter",
            "B Copter",
            MT::Air,
            6,
            3,
            99,
            6,
            9000,
            WT::AirToAir,
            WT::MachineGun,
            1,
            1,
            false,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "fighter",
            "Fighter",
            MT::Air,
            9,
            2,
            99,
            9,
            20000,
            WT::AirToAir,
            WT::None,
            1,
            1,
            false,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "bomber",
            "Bomber",
            MT::Air,
            7,
            2,
            99,
            9,
            22000,
            WT::Bombs,
            WT::None,
            1,
            1,
            false,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "stealth",
            "Stealth",
            MT::Air,
            6,
            4,
            60,
            6,
            24000,
            WT::Bombs,
            WT::AirToAir,
            1,
            1,
            false,
            true,
            0,
            {}
        ));
        add(makeUnit(
            "black_bomb",
            "Black Bomb",
            MT::Air,
            9,
            1,
            45,
            -1,
            25000,
            WT::None,
            WT::None,
            1,
            1,
            false,
            false,
            0,
            {}
        ));

        add(makeUnit(
            "battleship",
            "Battleship",
            MT::Sea,
            5,
            2,
            99,
            9,
            28000,
            WT::AntiShip,
            WT::None,
            2,
            6,
            false,
            false,
            0,
            {}
        ));
        add(makeUnit(
            "cruiser",
            "Cruiser",
            MT::Sea,
            6,
            3,
            99,
            9,
            18000,
            WT::AntiAir,
            WT::Torpedoes,
            1,
            1,
            false,
            true,
            2,
            {"b_copter", "t_copter"},
            true
        ));
        add(makeUnit(
            "submarine",
            "Submarine",
            MT::Sea,
            5,
            5,
            60,
            6,
            20000,
            WT::Torpedoes,
            WT::None,
            1,
            1,
            false,
            true,
            0,
            {}
        ));

        DamageTable& dmg = DamageTable::get();

        dmg.set("infantry", "infantry", 55);
        dmg.set("infantry", "mech", 45);
        dmg.set("infantry", "recon", 12);
        dmg.set("infantry", "tank", 5);
        dmg.set("infantry", "md_tank", 1);
        dmg.set("infantry", "apc", 14);
        dmg.set("infantry", "artillery", 15);
        dmg.set("infantry", "rockets", 25);
        dmg.set("infantry", "anti_air", 5);
        dmg.set("infantry", "missiles", 26);

        dmg.set("mech", "infantry", 65);
        dmg.set("mech", "mech", 55);
        dmg.set("mech", "recon", 85);
        dmg.set("mech", "tank", 55);
        dmg.set("mech", "md_tank", 15);
        dmg.set("mech", "apc", 75);
        dmg.set("mech", "artillery", 70);
        dmg.set("mech", "rockets", 85);
        dmg.set("mech", "anti_air", 65);
        dmg.set("mech", "missiles", 85);

        dmg.set("recon", "infantry", 70);
        dmg.set("recon", "mech", 65);
        dmg.set("recon", "recon", 35);
        dmg.set("recon", "apc", 45);
        dmg.set("recon", "artillery", 45);
        dmg.set("recon", "anti_air", 4);

        dmg.set("tank", "infantry", 75);
        dmg.set("tank", "mech", 70);
        dmg.set("tank", "recon", 85);
        dmg.set("tank", "tank", 55);
        dmg.set("tank", "md_tank", 15);
        dmg.set("tank", "neotank", 1);
        dmg.set("tank", "apc", 75);
        dmg.set("tank", "artillery", 70);
        dmg.set("tank", "rockets", 85);
        dmg.set("tank", "anti_air", 65);
        dmg.set("tank", "missiles", 85);
        dmg.set("tank", "pipe_runner", 25);

        dmg.set("md_tank", "infantry", 105);
        dmg.set("md_tank", "mech", 95);
        dmg.set("md_tank", "recon", 105);
        dmg.set("md_tank", "tank", 85);
        dmg.set("md_tank", "md_tank", 55);
        dmg.set("md_tank", "neotank", 15);
        dmg.set("md_tank", "megatank", 1);
        dmg.set("md_tank", "apc", 105);
        dmg.set("md_tank", "artillery", 105);
        dmg.set("md_tank", "rockets", 105);
        dmg.set("md_tank", "anti_air", 105);
        dmg.set("md_tank", "missiles", 105);
        dmg.set("md_tank", "pipe_runner", 45);

        dmg.set("neotank", "infantry", 125);
        dmg.set("neotank", "mech", 115);
        dmg.set("neotank", "recon", 125);
        dmg.set("neotank", "tank", 105);
        dmg.set("neotank", "md_tank", 75);
        dmg.set("neotank", "neotank", 55);
        dmg.set("neotank", "megatank", 15);
        dmg.set("neotank", "apc", 125);
        dmg.set("neotank", "artillery", 125);
        dmg.set("neotank", "rockets", 125);
        dmg.set("neotank", "anti_air", 115);
        dmg.set("neotank", "missiles", 125);
        dmg.set("neotank", "pipe_runner", 55);

        dmg.set("megatank", "infantry", 195);
        dmg.set("megatank", "mech", 195);
        dmg.set("megatank", "recon", 195);
        dmg.set("megatank", "tank", 180);
        dmg.set("megatank", "md_tank", 125);
        dmg.set("megatank", "neotank", 95);
        dmg.set("megatank", "megatank", 55);
        dmg.set("megatank", "apc", 195);
        dmg.set("megatank", "artillery", 195);
        dmg.set("megatank", "rockets", 195);
        dmg.set("megatank", "anti_air", 195);
        dmg.set("megatank", "missiles", 195);
        dmg.set("megatank", "pipe_runner", 125);

        dmg.set("anti_air", "infantry", 105);
        dmg.set("anti_air", "mech", 105);
        dmg.set("anti_air", "recon", 60);
        dmg.set("anti_air", "b_copter", 120);
        dmg.set("anti_air", "t_copter", 120);
        dmg.set("anti_air", "fighter", 65);
        dmg.set("anti_air", "bomber", 75);
        dmg.set("anti_air", "stealth", 75);
        dmg.set("anti_air", "black_bomb", 120);

        dmg.set("artillery", "infantry", 90);
        dmg.set("artillery", "mech", 85);
        dmg.set("artillery", "recon", 80);
        dmg.set("artillery", "tank", 70);
        dmg.set("artillery", "md_tank", 45);
        dmg.set("artillery", "apc", 70);
        dmg.set("artillery", "artillery", 75);
        dmg.set("artillery", "rockets", 80);
        dmg.set("artillery", "anti_air", 70);

        dmg.set("rockets", "infantry", 95);
        dmg.set("rockets", "mech", 90);
        dmg.set("rockets", "recon", 90);
        dmg.set("rockets", "tank", 80);
        dmg.set("rockets", "md_tank", 55);
        dmg.set("rockets", "apc", 80);
        dmg.set("rockets", "artillery", 80);
        dmg.set("rockets", "rockets", 85);
        dmg.set("rockets", "anti_air", 85);

        dmg.set("missiles", "b_copter", 120);
        dmg.set("missiles", "t_copter", 120);
        dmg.set("missiles", "fighter", 100);
        dmg.set("missiles", "bomber", 100);
        dmg.set("missiles", "stealth", 100);
        dmg.set("missiles", "black_bomb", 120);

        dmg.set("pipe_runner", "infantry", 105);
        dmg.set("pipe_runner", "mech", 105);
        dmg.set("pipe_runner", "recon", 80);
        dmg.set("pipe_runner", "tank", 85);
        dmg.set("pipe_runner", "md_tank", 60);
        dmg.set("pipe_runner", "neotank", 40);
        dmg.set("pipe_runner", "megatank", 15);
        dmg.set("pipe_runner", "apc", 90);
        dmg.set("pipe_runner", "artillery", 80);
        dmg.set("pipe_runner", "rockets", 85);
        dmg.set("pipe_runner", "anti_air", 75);
        dmg.set("pipe_runner", "missiles", 85);
        dmg.set("pipe_runner", "pipe_runner", 55);

        dmg.set("b_copter", "infantry", 75);
        dmg.set("b_copter", "mech", 75);
        dmg.set("b_copter", "recon", 55);
        dmg.set("b_copter", "tank", 55);
        dmg.set("b_copter", "md_tank", 25);
        dmg.set("b_copter", "apc", 60);
        dmg.set("b_copter", "artillery", 65);
        dmg.set("b_copter", "anti_air", 25);
        dmg.set("b_copter", "b_copter", 65);
        dmg.set("b_copter", "t_copter", 95);

        dmg.set("fighter", "b_copter", 100);
        dmg.set("fighter", "t_copter", 100);
        dmg.set("fighter", "fighter", 55);
        dmg.set("fighter", "bomber", 100);
        dmg.set("fighter", "stealth", 85);
        dmg.set("fighter", "black_bomb", 120);

        dmg.set("bomber", "infantry", 110);
        dmg.set("bomber", "mech", 110);
        dmg.set("bomber", "recon", 105);
        dmg.set("bomber", "tank", 105);
        dmg.set("bomber", "md_tank", 95);
        dmg.set("bomber", "neotank", 90);
        dmg.set("bomber", "megatank", 35);
        dmg.set("bomber", "apc", 105);
        dmg.set("bomber", "artillery", 105);
        dmg.set("bomber", "rockets", 105);
        dmg.set("bomber", "anti_air", 95);
        dmg.set("bomber", "missiles", 105);

        dmg.set("stealth", "infantry", 90);
        dmg.set("stealth", "mech", 90);
        dmg.set("stealth", "recon", 85);
        dmg.set("stealth", "tank", 75);
        dmg.set("stealth", "md_tank", 70);
        dmg.set("stealth", "apc", 85);
        dmg.set("stealth", "artillery", 75);
        dmg.set("stealth", "b_copter", 85);
        dmg.set("stealth", "t_copter", 95);
        dmg.set("stealth", "fighter", 45);
        dmg.set("stealth", "bomber", 55);
        dmg.set("stealth", "stealth", 55);

        dmg.set("battleship", "recon", 90);
        dmg.set("battleship", "tank", 80);
        dmg.set("battleship", "md_tank", 55);
        dmg.set("battleship", "apc", 80);
        dmg.set("battleship", "artillery", 80);
        dmg.set("battleship", "rockets", 85);
        dmg.set("battleship", "cruiser", 95);
        dmg.set("battleship", "submarine", 95);
        dmg.set("battleship", "battleship", 50);
        dmg.set("battleship", "lander", 95);
        dmg.set("battleship", "black_boat", 95);
        dmg.set("battleship", "carrier", 75);

        dmg.set("cruiser", "b_copter", 115);
        dmg.set("cruiser", "t_copter", 115);
        dmg.set("cruiser", "fighter", 55);
        dmg.set("cruiser", "bomber", 65);
        dmg.set("cruiser", "stealth", 65);
        dmg.set("cruiser", "submarine", 90);

        dmg.set("submarine", "battleship", 55);
        dmg.set("submarine", "cruiser", 25);
        dmg.set("submarine", "submarine", 55);
        dmg.set("submarine", "lander", 95);
        dmg.set("submarine", "black_boat", 95);
        dmg.set("submarine", "carrier", 75);
    }

} // namespace OpenWars::Game