#include "map.hpp"

namespace OpenWars::Game {

    Map::Map(int width, int height)
        : width(width)
        , height(height) {
        // Initialize all tiles to Plain terrain with 0 defense
        tiles.resize(height);
        for(int y = 0; y < height; ++y) {
            tiles[y].resize(width);
            for(int x = 0; x < width; ++x) {
                tiles[y][x] = std::make_unique<Terrain>(TerrainType::Plain, 0);
            }
        }
    }

    int Map::getWidth() const {
        return width;
    }

    int Map::getHeight() const {
        return height;
    }

    bool Map::isInBounds(const Vector2& pos) const {
        return isInBounds((int)pos.x, (int)pos.y);
    }

    bool Map::isInBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    Terrain* Map::getTerrain(int x, int y) const {
        if(!isInBounds(x, y))
            return nullptr;
        return tiles[y][x].get();
    }

    Terrain* Map::getTerrain(const Vector2& pos) const {
        return getTerrain((int)pos.x, (int)pos.y);
    }

    void Map::setTerrain(int x, int y, TerrainType type, int defense) {
        if(!isInBounds(x, y))
            return;
        tiles[y][x] = std::make_unique<Terrain>(type, defense);
    }

    void Map::setTerrain(const Vector2& pos, TerrainType type, int defense) {
        setTerrain((int)pos.x, (int)pos.y, type, defense);
    }

    int
    Map::getMovementCost(const Vector2& pos, MovementType movementType) const {
        Terrain* terrain = getTerrain(pos);
        if(!terrain)
            return -1; // Out of bounds, impassable

        TerrainType type = terrain->getType();

        // Movement costs depend on movement type and terrain
        // Based on Advance Wars mechanics
        switch(movementType) {
        case MovementType::Infantry:
        case MovementType::Mech:
            switch(type) {
            case TerrainType::Plain:
            case TerrainType::Road:
            case TerrainType::Beach:
            case TerrainType::City:
            case TerrainType::Base:
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
            case TerrainType::Shoal:
            case TerrainType::Reef:
                return -1; // Impassable
            }
            break;

        case MovementType::TireA:
            switch(type) {
            case TerrainType::Plain:
            case TerrainType::Road:
            case TerrainType::Beach:
            case TerrainType::City:
            case TerrainType::Base:
            case TerrainType::Airport:
            case TerrainType::Port:
            case TerrainType::HQ:
            case TerrainType::Lab:
            case TerrainType::CommTower:
                return 1;
            case TerrainType::Wood:
            case TerrainType::Shoal:
                return 3;
            case TerrainType::Mountain:
            case TerrainType::Silo:
                return -1; // Impassable
            case TerrainType::River:
            case TerrainType::Sea:
            case TerrainType::Reef:
            case TerrainType::Pipe:
                return -1; // Impassable
            }
            break;

        case MovementType::Tread:
            switch(type) {
            case TerrainType::Plain:
            case TerrainType::Road:
            case TerrainType::Beach:
            case TerrainType::City:
            case TerrainType::Base:
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
            case TerrainType::Shoal:
            case TerrainType::Reef:
            case TerrainType::Pipe:
                return -1; // Impassable
            }
            break;

        case MovementType::Air:
            // Air units can move over most terrain except mountains
            switch(type) {
            case TerrainType::Mountain:
                return -1; // Impassable
            default:
                return 1; // All other terrain costs 1 for air
            }

        case MovementType::Sea:
            switch(type) {
            case TerrainType::Sea:
            case TerrainType::Shoal:
            case TerrainType::Reef:
            case TerrainType::River:
                return 1;
            case TerrainType::Port:
            case TerrainType::Base:
                return 1;
            default:
                return -1; // Impassable on land
            }

        case MovementType::Lander:
            // Landers can move on water and beach
            switch(type) {
            case TerrainType::Sea:
            case TerrainType::Shoal:
            case TerrainType::Beach:
                return 1;
            case TerrainType::Port:
            case TerrainType::Base:
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
    }

    bool Map::isPassable(const Vector2& pos, MovementType movementType) const {
        int cost = getMovementCost(pos, movementType);
        return cost >= 0;
    }

    std::vector<Vector2> Map::getTilesByType(TerrainType type) const {
        std::vector<Vector2> result;
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                if(tiles[y][x]->getType() == type) {
                    result.push_back({(float)x, (float)y});
                }
            }
        }
        return result;
    }

    void Map::fillRectangle(
        int x,
        int y,
        int w,
        int h,
        TerrainType type,
        int defense
    ) {
        for(int ty = y; ty < y + h; ++ty) {
            for(int tx = x; tx < x + w; ++tx) {
                if(isInBounds(tx, ty)) {
                    setTerrain(tx, ty, type, defense);
                }
            }
        }
    }

    void Map::clear() {
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                tiles[y][x] = std::make_unique<Terrain>(TerrainType::Plain, 0);
            }
        }
    }

    float Map::getAverageDefense(int x, int y, int radius) const {
        float sum = 0.0f;
        int count = 0;

        for(int ty = y - radius; ty <= y + radius; ++ty) {
            for(int tx = x - radius; tx <= x + radius; ++tx) {
                if(isInBounds(tx, ty)) {
                    Terrain* terrain = getTerrain(tx, ty);
                    if(terrain) {
                        sum += terrain->getDefenseStars();
                        count++;
                    }
                }
            }
        }

        return count > 0 ? sum / count : 0.0f;
    }

    void
    Map::forEachTile(std::function<void(int, int, Terrain*)> callback) const {
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                callback(x, y, tiles[y][x].get());
            }
        }
    }

} // namespace OpenWars::Game
