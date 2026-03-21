#include "map.hpp"
#include "tiles.hpp"

namespace OpenWars::Game {

    Map::Map(int width, int height)
        : width(width)
        , height(height) {
        Tiles::registerAll();
        tiles.resize(width * height, Terrain{Tiles::Plain});
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

    Terrain* Map::getTerrain(int x, int y) {
        if(!isInBounds(x, y))
            return nullptr;
        return &tiles[index(x, y)];
    }

    const Terrain* Map::getTerrain(int x, int y) const {
        if(!isInBounds(x, y))
            return nullptr;
        return &tiles[index(x, y)];
    }

    const Terrain* Map::getTerrain(const Vector2& pos) const {
        return getTerrain((int)pos.x, (int)pos.y);
    }

    void Map::setTerrain(int x, int y, TileTypeID id) {
        if(!isInBounds(x, y))
            return;
        tiles[index(x, y)] = Terrain{id};
    }

    void Map::setTerrain(const Vector2& pos, TileTypeID id) {
        setTerrain((int)pos.x, (int)pos.y, id);
    }

    int Map::getMovementCost(
        const Vector2& pos,
        MovementType movementType
    ) const {
        const Terrain* t = getTerrain(pos);
        return t ? t->getMovementCost(movementType) : -1;
    }

    bool Map::isPassable(const Vector2& pos, MovementType movementType) const {
        const Terrain* t = getTerrain(pos);
        return t && t->isPassable(movementType);
    }

    std::vector<Vector2> Map::getTilesByType(TileTypeID id) const {
        std::vector<Vector2> result;
        for(int y = 0; y < height; ++y)
            for(int x = 0; x < width; ++x)
                if(tiles[index(x, y)].typeId == id)
                    result.push_back({(float)x, (float)y});
        return result;
    }

    void Map::fillRectangle(int x, int y, int w, int h, TileTypeID id) {
        for(int ty = y; ty < y + h; ++ty)
            for(int tx = x; tx < x + w; ++tx)
                if(isInBounds(tx, ty))
                    setTerrain(tx, ty, id);
    }

    void Map::clear() {
        std::fill(tiles.begin(), tiles.end(), Terrain{Game::Tiles::Plain});
    }

    float Map::getAverageDefense(int x, int y, int radius) const {
        float sum = 0.0f;
        int count = 0;
        for(int ty = y - radius; ty <= y + radius; ++ty) {
            for(int tx = x - radius; tx <= x + radius; ++tx) {
                if(isInBounds(tx, ty)) {
                    sum += tiles[index(tx, ty)].getDefenseStars();
                    ++count;
                }
            }
        }
        return count > 0 ? sum / count : 0.0f;
    }

    void Map::forEachTile(
        std::function<void(int, int, const Terrain*)> callback
    ) const {
        for(int y = 0; y < height; ++y)
            for(int x = 0; x < width; ++x)
                callback(x, y, &tiles[index(x, y)]);
    }

} // namespace OpenWars::Game