#pragma once
#include "tiletype.hpp"

namespace OpenWars::Game::Tiles {
    extern TileTypeID Plain;
    extern TileTypeID Road;
    extern TileTypeID Bridge;
    extern TileTypeID Woods;
    extern TileTypeID Mountain;
    extern TileTypeID HighMountain;
    extern TileTypeID River;
    extern TileTypeID Coast;
    extern TileTypeID Sea;
    extern TileTypeID Reef;
    extern TileTypeID City;
    extern TileTypeID Factory;
    extern TileTypeID Airport;
    extern TileTypeID Port;
    extern TileTypeID HQ;
    extern TileTypeID CommTower;
    extern TileTypeID Lab;
    extern TileTypeID Pipe;
    extern TileTypeID Silo;

    void registerAll();
} // namespace OpenWars::Game::Tiles