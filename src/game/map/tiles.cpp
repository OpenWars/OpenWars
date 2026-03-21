#include "tiles.hpp"
#include "../movement.hpp"

namespace OpenWars::Game::Tiles {
    TileTypeID Plain;
    TileTypeID Road;
    TileTypeID Bridge;
    TileTypeID Woods;
    TileTypeID Mountain;
    TileTypeID HighMountain;
    TileTypeID River;
    TileTypeID Coast;
    TileTypeID Sea;
    TileTypeID Reef;
    TileTypeID City;
    TileTypeID Factory;
    TileTypeID Airport;
    TileTypeID Port;
    TileTypeID HQ;
    TileTypeID CommTower;
    TileTypeID Lab;
    TileTypeID Pipe;
    TileTypeID Silo;

    static const int CG_ROAD = 1;
    static const int CG_RIVER = 2;
    static const int CG_SEA = 3;
    static const int CG_COAST = 4;

    // Mask bits: N=bit0 S=bit1 E=bit2 W=bit3

    static TileConnectionSprites buildRoadSprites() {
        TileConnectionSprites s;
        s.byMask[0] = {2, 9, 0};   // isolated
        s.byMask[1] = {2, 9, 1};   // N
        s.byMask[2] = {2, 9, 1};   // S
        s.byMask[3] = {2, 9, 1};   // NS
        s.byMask[4] = {2, 9, 0};   // E
        s.byMask[5] = {2, 8, 3};   // NE
        s.byMask[6] = {2, 8, 0};   // SE
        s.byMask[7] = {2, 10, 3};  // NSE
        s.byMask[8] = {2, 9, 0};   // W
        s.byMask[9] = {2, 8, 2};   // NW
        s.byMask[10] = {2, 8, 1};  // SW
        s.byMask[11] = {2, 10, 1}; // NSW
        s.byMask[12] = {2, 9, 0};  // EW
        s.byMask[13] = {2, 10, 2}; // NEW
        s.byMask[14] = {2, 10, 0}; // SEW
        s.byMask[15] = {3, 10, 0}; // NSEW
        return s;
    }

    static TileConnectionSprites buildRiverSprites() {
        TileConnectionSprites s;
        s.byMask[0] = {4, 3, 1};  // isolated
        s.byMask[1] = {4, 3, 0};  // N
        s.byMask[2] = {4, 3, 0};  // S
        s.byMask[3] = {5, 3, 0};  // NS
        s.byMask[4] = {4, 3, 1};  // E
        s.byMask[5] = {5, 4, 3};  // NE
        s.byMask[6] = {5, 4, 0};  // SE
        s.byMask[7] = {6, 4, 1};  // NSE
        s.byMask[8] = {4, 3, 1};  // W
        s.byMask[9] = {5, 4, 2};  // NW
        s.byMask[10] = {5, 4, 1}; // SW
        s.byMask[11] = {6, 4, 2}; // NSW
        s.byMask[12] = {4, 3, 0}; // EW
        s.byMask[13] = {6, 4, 0}; // NEW
        s.byMask[14] = {6, 4, 3}; // SEW
        s.byMask[15] = {6, 3, 0}; // NSEW
        return s;
    }

    static TileConnectionSprites buildCoastSprites() {
        TileConnectionSprites s;
        s.byMask[0] = {6, 1, 0};  // no sea
        s.byMask[1] = {6, 1, 3};  // N
        s.byMask[2] = {6, 1, 1};  // S
        s.byMask[3] = {6, 1, 3};  // NS → N approx
        s.byMask[4] = {6, 1, 0};  // E
        s.byMask[5] = {5, 1, 3};  // NE
        s.byMask[6] = {5, 1, 0};  // SE
        s.byMask[7] = {6, 1, 0};  // NSE → E approx
        s.byMask[8] = {6, 1, 2};  // W
        s.byMask[9] = {5, 1, 2};  // NW
        s.byMask[10] = {5, 1, 1}; // SW
        s.byMask[11] = {6, 1, 2}; // NSW → W approx
        s.byMask[12] = {6, 1, 0}; // EW → E approx
        s.byMask[13] = {6, 1, 3}; // NEW → N approx
        s.byMask[14] = {6, 1, 1}; // SEW → S approx
        s.byMask[15] = {6, 1, 0}; // NSEW
        return s;
    }

    using MT = MovementType;

    void registerAll() {
        static bool done = false;
        if(done)
            return;
        done = true;

        auto& reg = TileRegistry::get();

        {
            TileDefinition def;
            def.name = "Plain";
            def.defenseStars = 1;
            def.sprite = {1, 1};
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, 1},
                {MT::Tread, 1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            Plain = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Road";
            def.defenseStars = 0;
            def.sprite = {2, 9};
            def.foreground = true;
            def.connectionGroup = CG_ROAD;
            def.connectsTo = {CG_ROAD};
            def.connectionSprites = buildRoadSprites();
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, 1},
                {MT::Tread, 1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            Road = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Bridge";
            def.defenseStars = 0;
            def.sprite = {4, 8};
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, 1},
                {MT::Tread, 1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            Bridge = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Woods";
            def.defenseStars = 2;
            def.sprite = {1, 4, 1, 3}; // even-y: col 4; odd-y: col 3
            def.foreground = true;
            def.movementCosts = {
                {MT::Infantry, 2},
                {MT::Mech, 2},
                {MT::Tire, 3},
                {MT::Tread, 2},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            Woods = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Mountain";
            def.defenseStars = 4;
            def.sprite = {2, 5};
            def.foreground = true;
            def.movementCosts = {
                {MT::Infantry, 4},
                {MT::Mech, 4},
                {MT::Tire, -1},
                {MT::Tread, -1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            Mountain = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "HighMountain";
            def.defenseStars = 0;
            def.sprite = {2, 6};
            def.overlay = {1, 6};
            def.foreground = true;
            def.movementCosts = {
                {MT::Infantry, 4},
                {MT::Mech, 4},
                {MT::Tire, -1},
                {MT::Tread, -1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            HighMountain = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "River";
            def.defenseStars = 0;
            def.sprite = {5, 3};
            def.connectionGroup = CG_RIVER;
            def.connectsTo = {CG_RIVER, CG_SEA, CG_COAST};
            def.connectionSprites = buildRiverSprites();
            def.movementCosts = {
                {MT::Infantry, -1},
                {MT::Mech, -1},
                {MT::Tire, -1},
                {MT::Tread, -1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            River = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Coast";
            def.defenseStars = 0;
            def.sprite = {5, 1};
            def.underlay = {3, 1, 0, 0, 3, 1.0f}; // animated sea beneath
            def.foreground = true;
            def.connectionGroup = CG_COAST;
            def.connectsTo = {CG_SEA, CG_RIVER};
            def.diagonalConnectionFallback = true;
            def.connectionSprites = buildCoastSprites();
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, 1},
                {MT::Tread, 1},
                {MT::Air, 1},
                {MT::Sea, 1},
                {MT::Lander, 1},
                {MT::Pipe, -1}
            };
            Coast = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Sea";
            def.defenseStars = 0;
            def.sprite = {3, 1, 0, 0, 3, 1.0f}; // animated, 3 frames
            def.connectionGroup = CG_SEA;
            def.movementCosts = {
                {MT::Infantry, -1},
                {MT::Mech, -1},
                {MT::Tire, -1},
                {MT::Tread, -1},
                {MT::Air, 1},
                {MT::Sea, 1},
                {MT::Lander, 1},
                {MT::Pipe, -1}
            };
            Sea = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Reef";
            def.defenseStars = 1;
            def.sprite = {3, 4};
            def.underlay = {3, 1, 0, 0, 3, 1.0f}; // animated sea beneath
            def.foreground = true;
            def.connectionGroup = CG_SEA;
            def.movementCosts = {
                {MT::Infantry, -1},
                {MT::Mech, -1},
                {MT::Tire, -1},
                {MT::Tread, -1},
                {MT::Air, 1},
                {MT::Sea, 1},
                {MT::Lander, 1},
                {MT::Pipe, -1}
            };
            Reef = reg.registerTile(std::move(def));
        }

        // Structures share movement cost pattern and CG_ROAD for road
        // connectivity. Sprites are placeholders (plain) until spritesheet
        // support is added.

        {
            TileDefinition def;
            def.name = "City";
            def.defenseStars = 3;
            def.sprite = {1, 1};
            def.connectionGroup = CG_ROAD;
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, 1},
                {MT::Tread, 1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            City = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Factory";
            def.defenseStars = 3;
            def.sprite = {1, 1};
            def.foreground = true;
            def.connectionGroup = CG_ROAD;
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, 1},
                {MT::Tread, 1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            Factory = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Airport";
            def.defenseStars = 3;
            def.sprite = {1, 1};
            def.foreground = true;
            def.connectionGroup = CG_ROAD;
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, 1},
                {MT::Tread, 1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            Airport = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Port";
            def.defenseStars = 3;
            def.sprite = {1, 1};
            def.foreground = true;
            def.connectionGroup = CG_ROAD;
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, 1},
                {MT::Tread, 1},
                {MT::Air, 1},
                {MT::Sea, 1},
                {MT::Lander, 1},
                {MT::Pipe, -1}
            };
            Port = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "HQ";
            def.defenseStars = 4;
            def.sprite = {1, 1};
            def.foreground = true;
            def.connectionGroup = CG_ROAD;
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, 1},
                {MT::Tread, 1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            HQ = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "CommTower";
            def.defenseStars = 3;
            def.sprite = {1, 1};
            def.foreground = true;
            def.connectionGroup = CG_ROAD;
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, 1},
                {MT::Tread, 1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            CommTower = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Lab";
            def.defenseStars = 3;
            def.sprite = {1, 1};
            def.foreground = true;
            def.connectionGroup = CG_ROAD;
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, 1},
                {MT::Tread, 1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            Lab = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Pipe";
            def.defenseStars = 0;
            def.sprite = {1, 1};
            def.movementCosts = {
                {MT::Infantry, 1},
                {MT::Mech, 1},
                {MT::Tire, -1},
                {MT::Tread, -1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, 1}
            };
            Pipe = reg.registerTile(std::move(def));
        }

        {
            TileDefinition def;
            def.name = "Silo";
            def.defenseStars = 3;
            def.sprite = {1, 1};
            def.foreground = true;
            def.movementCosts = {
                {MT::Infantry, 2},
                {MT::Mech, 2},
                {MT::Tire, -1},
                {MT::Tread, -1},
                {MT::Air, 1},
                {MT::Sea, -1},
                {MT::Lander, -1},
                {MT::Pipe, -1}
            };
            Silo = reg.registerTile(std::move(def));
        }
    }
} // namespace OpenWars::Game::Tiles