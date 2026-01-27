#include "terrain.hpp"
#include "../units/unit.hpp"

int OpenWars::Game::Terrain::getMovementCost(MovementType movementType) {
    switch(movementType) {
    case MovementType::Infantry:
    case MovementType::Mech:
        switch(type) {
        case TerrainType::Plain:
        case TerrainType::Road:
        case TerrainType::Coast:
        case TerrainType::City:
        case TerrainType::Factory:
        case TerrainType::Airport:
        case TerrainType::Port:
        case TerrainType::Lab:
        case TerrainType::CommTower:
        case TerrainType::HQ:
            return 1;
        case TerrainType::Wood:
        case TerrainType::Silo:
            return 2;
        case TerrainType::Mountain:
            return 4;
        case TerrainType::Pipe:
            return 1;
        case TerrainType::River:
        case TerrainType::Sea:
            return -1; // Impassable
        }
        break;

    case MovementType::Tire:
        switch(type) {
        case TerrainType::Plain:
        case TerrainType::Road:
        case TerrainType::Coast:
        case TerrainType::City:
        case TerrainType::Airport:
        case TerrainType::Port:
        case TerrainType::HQ:
        case TerrainType::Lab:
        case TerrainType::CommTower:
        case TerrainType::Factory:
            return 1;
        case TerrainType::Wood:
            return 3;
        case TerrainType::Mountain:
        case TerrainType::Silo:
            return -1; // Impassable
        case TerrainType::River:
        case TerrainType::Sea:
        case TerrainType::Pipe:
            return -1; // Impassable
        }
        break;

    case MovementType::Tread:
        switch(type) {
        case TerrainType::Factory:
        case TerrainType::Plain:
        case TerrainType::Road:
        case TerrainType::Coast:
        case TerrainType::City:
        case TerrainType::Airport:
        case TerrainType::Port:
        case TerrainType::HQ:
        case TerrainType::Lab:
        case TerrainType::CommTower:
            return 1;
        case TerrainType::Wood:
            return 2;
        case TerrainType::Mountain:
        case TerrainType::Silo:
            return -1; // Impassable
        case TerrainType::River:
        case TerrainType::Sea:
        case TerrainType::Pipe:
            return -1; // Impassable
        }
        break;

    case MovementType::Air:
        return 1;

    case MovementType::Sea:
        switch(type) {
        case TerrainType::Sea:
        case TerrainType::Coast:
            return 1;
        case TerrainType::Port:
            return 1;
        default:
            return -1; // Impassable on land
        }

    case MovementType::Lander:
        switch(type) {
        case TerrainType::Sea:
        case TerrainType::Coast:
        case TerrainType::Port:
            return 1;
        default:
            return -1;
        }

    case MovementType::Pipe:
        // Pipe runners move on pipes
        switch(type) {
        case TerrainType::Pipe:
            return 1;
        default:
            return -1; // Only on pipes
        }
    }

    return -1; // Default: impassable
};