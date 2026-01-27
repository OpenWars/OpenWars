#include "map.hpp"
#include "terrain.hpp"

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

    // Alias for Terrain->getMovementCost
    int
    Map::getMovementCost(const Vector2& pos, MovementType movementType) const {
        Terrain* terrain = getTerrain(pos);
        if(!terrain)
            return -1; // Out of bounds, impassable

        return terrain->getMovementCost(movementType);
    }

    // Alias for Terrain->isPassable
    bool Map::isPassable(const Vector2& pos, MovementType movementType) const {
        Terrain* terrain = getTerrain(pos);
        if(!terrain)
            return false; // Out of bounds, impassable

        return terrain->isPassable(movementType);
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
