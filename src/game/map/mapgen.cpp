#include "mapgen.hpp"
#include "tiles.hpp"
#include <random>
#include <cmath>
#include <algorithm>

namespace OpenWars::Game {

    static std::mt19937 generator(42);
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    float MapGenerator::perlinNoise(int x, int y, int seed) {
        int n = std::sin(x * 12.9898f + y * 78.233f + seed) * 43758.5453f;
        return (float)(n - std::floor(n));
    }

    void MapGenerator::smoothTerrain(Map* map, int radius) {
        const int width = map->getWidth();
        const int height = map->getHeight();
        std::vector<std::vector<TileTypeID>> smoothed(
            height,
            std::vector<TileTypeID>(width)
        );

        map->forEachTile([&](int x, int y, const Terrain* t) {
            int waterCount = 0;
            int landCount = 0;

            for(int dy = -radius; dy <= radius; ++dy) {
                for(int dx = -radius; dx <= radius; ++dx) {
                    const Terrain* neighbor = map->getTerrain(x + dx, y + dy);
                    if(neighbor) {
                        TileTypeID id = neighbor->typeId;
                        if(id == Tiles::Sea || id == Tiles::River)
                            waterCount++;
                        else if(id != Tiles::Mountain)
                            landCount++;
                    }
                }
            }

            TileTypeID id = t->typeId;
            if((id == Tiles::Plain || id == Tiles::Woods) &&
               waterCount > landCount)
                smoothed[y][x] = Tiles::Coast;
            else
                smoothed[y][x] = id;
        });

        for(int y = 0; y < height; ++y)
            for(int x = 0; x < width; ++x)
                map->setTerrain(x, y, smoothed[y][x]);
    }

    void MapGenerator::addStructures(Map* map, int count, TileTypeID type) {
        std::uniform_int_distribution<> widthDist(0, map->getWidth() - 1);
        std::uniform_int_distribution<> heightDist(0, map->getHeight() - 1);

        for(int i = 0; i < count; ++i) {
            int x = widthDist(generator);
            int y = heightDist(generator);
            const Terrain* terrain = map->getTerrain(x, y);
            if(terrain && terrain->typeId == Tiles::Plain)
                map->setTerrain(x, y, type);
        }
    }

    void MapGenerator::addRoads(Map* map) {
        for(int y = 5; y < map->getHeight(); y += 8) {
            for(int x = 0; x < map->getWidth(); ++x) {
                const Terrain* terrain = map->getTerrain(x, y);
                if(terrain && terrain->typeId == Tiles::Plain)
                    map->setTerrain(x, y, Tiles::Road);
            }
        }
    }

    std::unique_ptr<Map> MapGenerator::generateRandom(int width, int height) {
        auto map = std::make_unique<Map>(width, height);
        std::uniform_real_distribution<> terrainDist(0.0, 1.0);

        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                float noise = terrainDist(generator);
                TileTypeID type;
                if(noise < 0.3f)
                    type = Tiles::Sea;
                else if(noise < 0.4f)
                    type = Tiles::Coast;
                else if(noise < 0.65f)
                    type = Tiles::Plain;
                else if(noise < 0.85f)
                    type = Tiles::Woods;
                else
                    type = Tiles::Mountain;
                map->setTerrain(x, y, type);
            }
        }

        smoothTerrain(map.get());
        return map;
    }

    std::unique_ptr<Map>
    MapGenerator::generateIslandMap(int width, int height, int islandCount) {
        auto map = std::make_unique<Map>(width, height);

        for(int y = 0; y < height; ++y)
            for(int x = 0; x < width; ++x)
                map->setTerrain(x, y, Tiles::Sea);

        std::uniform_int_distribution<> widthDist(5, width - 5);
        std::uniform_int_distribution<> heightDist(5, height - 5);
        std::uniform_int_distribution<> sizeDist(3, 8);

        for(int i = 0; i < islandCount; ++i) {
            int cx = widthDist(generator);
            int cy = heightDist(generator);
            int size = sizeDist(generator);

            for(int y = cy - size; y <= cy + size; ++y) {
                for(int x = cx - size; x <= cx + size; ++x) {
                    if(map->isInBounds(x, y)) {
                        int dx = x - cx;
                        int dy = y - cy;
                        if(dx * dx + dy * dy <= size * size)
                            map->setTerrain(x, y, Tiles::Plain);
                    }
                }
            }
        }

        smoothTerrain(map.get());
        addStructures(map.get(), std::max(2, islandCount / 2), Tiles::City);
        return map;
    }

    std::unique_ptr<Map> MapGenerator::generateRiverMap(int width, int height) {
        auto map = std::make_unique<Map>(width, height);

        int numRivers = 2;
        for(int r = 0; r < numRivers; ++r) {
            int x = (r + 1) * width / (numRivers + 1);
            for(int y = 0; y < height; ++y) {
                if(map->isInBounds(x, y))
                    map->setTerrain(x, y, Tiles::River);
                if(distribution(generator) < 0.3f)
                    x += distribution(generator) < 0.5f ? 1 : -1;
            }
        }

        for(int i = 0; i < 15; ++i) {
            std::uniform_int_distribution<> widthDist(2, width - 3);
            std::uniform_int_distribution<> heightDist(2, height - 3);
            map->fillRectangle(
                widthDist(generator),
                heightDist(generator),
                3,
                3,
                Tiles::Woods
            );
        }

        addStructures(map.get(), 4, Tiles::City);
        return map;
    }

    std::unique_ptr<Map>
    MapGenerator::generateMountainMap(int width, int height) {
        auto map = std::make_unique<Map>(width, height);

        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                float noise = perlinNoise(x / 4, y / 4, 12) * 0.5f +
                              perlinNoise(x / 8, y / 8, 42) * 0.3f +
                              perlinNoise(x / 16, y / 16, 99) * 0.2f;

                TileTypeID type;
                if(noise < 0.2f)
                    type = Tiles::Mountain;
                else if(noise < 0.4f)
                    type = Tiles::Woods;
                else if(noise < 0.6f)
                    type = Tiles::Plain;
                else
                    type = Tiles::Road;
                map->setTerrain(x, y, type);
            }
        }

        smoothTerrain(map.get());
        addRoads(map.get());
        return map;
    }

    std::unique_ptr<Map> MapGenerator::generateTestMap(int width, int height) {
        auto map = std::make_unique<Map>(width, height);

        map->fillRectangle(0, 0, 4, 4, Tiles::Mountain);
        map->fillRectangle(0, 0, 4, 2, Tiles::HighMountain);
        map->fillRectangle(width - 4, 0, 4, 4, Tiles::Woods);
        map->fillRectangle(0, height - 4, 4, 4, Tiles::Sea);
        map->fillRectangle(0, height - 4, 2, 2, Tiles::Reef);

        for(int x = 0; x < 4; ++x)
            map->setTerrain(x, height - 5, Tiles::Coast);
        for(int y = height - 4; y < height; ++y)
            map->setTerrain(4, y, Tiles::Coast);
        map->setTerrain(4, height - 5, Tiles::Coast);
        map->setTerrain(4, height - 4, Tiles::Coast);

        for(int y = 0; y < height - 4; ++y)
            map->setTerrain(width / 4, y, Tiles::River);

        for(int x = 0; x < width; ++x)
            map->setTerrain(x, height / 2, Tiles::Road);
        for(int y = 0; y < height; ++y)
            map->setTerrain(width / 2, y, Tiles::Road);

        map->setTerrain(width / 4, height / 2, Tiles::Bridge);

        for(int x = 4; x < width / 2 - 1; ++x)
            map->setTerrain(x, 1, Tiles::Pipe);

        // Road connection-mask sampler (bottom-right quadrant)
        const int bx = width - 8;
        const int by = height - 9;

        map->setTerrain(bx, by, Tiles::Road);
        map->setTerrain(bx + 2, by, Tiles::Road);
        map->setTerrain(bx + 2, by + 1, Tiles::Road);
        map->setTerrain(bx, by + 2, Tiles::Road);
        map->setTerrain(bx + 1, by + 2, Tiles::Road);
        map->setTerrain(bx + 4, by, Tiles::Road);
        map->setTerrain(bx + 4, by + 1, Tiles::Road);
        map->setTerrain(bx + 3, by + 1, Tiles::Road);
        map->setTerrain(bx + 4, by + 2, Tiles::Road);
        map->setTerrain(bx + 5, by + 2, Tiles::Road);
        map->setTerrain(bx + 4, by + 3, Tiles::Road);
        map->setTerrain(bx, by + 4, Tiles::Road);
        map->setTerrain(bx + 1, by + 4, Tiles::Road);
        map->setTerrain(bx + 2, by + 4, Tiles::Road);
        map->setTerrain(bx + 1, by + 5, Tiles::Road);
        map->setTerrain(bx + 4, by + 4, Tiles::Road);
        map->setTerrain(bx + 3, by + 5, Tiles::Road);
        map->setTerrain(bx + 4, by + 5, Tiles::Road);
        map->setTerrain(bx + 5, by + 5, Tiles::Road);
        map->setTerrain(bx + 4, by + 6, Tiles::Road);

        // River connection-mask sampler (top-center strip)
        const int rx = width / 2 + 2;
        const int ry = 3;

        map->setTerrain(rx, ry, Tiles::River);
        map->setTerrain(rx, ry + 1, Tiles::River);
        map->setTerrain(rx + 2, ry, Tiles::River);
        map->setTerrain(rx + 3, ry, Tiles::River);
        map->setTerrain(rx + 5, ry, Tiles::River);
        map->setTerrain(rx + 5, ry + 1, Tiles::River);
        map->setTerrain(rx + 6, ry + 1, Tiles::River);
        map->setTerrain(rx + 8, ry, Tiles::River);
        map->setTerrain(rx + 8, ry + 1, Tiles::River);
        map->setTerrain(rx + 9, ry + 1, Tiles::River);
        map->setTerrain(rx + 11, ry, Tiles::River);
        map->setTerrain(rx + 11, ry + 1, Tiles::River);
        map->setTerrain(rx + 12, ry, Tiles::River);
        map->setTerrain(rx + 14, ry, Tiles::River);
        map->setTerrain(rx + 14, ry + 1, Tiles::River);
        map->setTerrain(rx + 15, ry, Tiles::River);
        map->setTerrain(rx + 17, ry - 1, Tiles::River);
        map->setTerrain(rx + 17, ry, Tiles::River);
        map->setTerrain(rx + 17, ry + 1, Tiles::River);
        map->setTerrain(rx + 18, ry, Tiles::River);
        map->setTerrain(rx + 20, ry - 1, Tiles::River);
        map->setTerrain(rx + 19, ry, Tiles::River);
        map->setTerrain(rx + 20, ry, Tiles::River);
        map->setTerrain(rx + 21, ry, Tiles::River);
        map->setTerrain(rx + 20, ry + 1, Tiles::River);

        map->setTerrain(1, 1, Tiles::HQ);
        map->setTerrain(3, height / 2 - 3, Tiles::Factory);
        map->setTerrain(5, height / 2 - 3, Tiles::City);
        map->setTerrain(3, height / 2 + 3, Tiles::Airport);
        map->setTerrain(5, height / 2 + 3, Tiles::Port);

        map->setTerrain(width - 2, height - 2, Tiles::HQ);
        map->setTerrain(width - 4, height / 2 - 3, Tiles::Factory);
        map->setTerrain(width - 6, height / 2 - 3, Tiles::City);
        map->setTerrain(width - 4, height / 2 + 3, Tiles::Airport);
        map->setTerrain(width - 6, height / 2 + 3, Tiles::Port);

        map->setTerrain(width / 2 - 3, height / 2 - 3, Tiles::CommTower);
        map->setTerrain(width / 2 + 3, height / 2 - 3, Tiles::CommTower);
        map->setTerrain(width / 2 - 3, height / 2 + 3, Tiles::Lab);
        map->setTerrain(width / 2 + 3, height / 2 + 3, Tiles::Silo);

        return map;
    }

    std::unique_ptr<Map>
    MapGenerator::generateSymmetricalMap(int width, int height) {
        auto map = std::make_unique<Map>(width, height);

        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width / 2; ++x) {
                float noise = perlinNoise(x, y, 15);
                TileTypeID type;
                if(noise < 0.3f)
                    type = Tiles::Mountain;
                else if(noise < 0.5f)
                    type = Tiles::Woods;
                else
                    type = Tiles::Plain;
                map->setTerrain(x, y, type);
            }
        }

        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width / 2; ++x) {
                const Terrain* t = map->getTerrain(width / 2 - 1 - x, y);
                if(t)
                    map->setTerrain(width / 2 + x, y, t->typeId);
            }
        }

        map->setTerrain(2, height / 2 - 1, Tiles::Factory);
        map->setTerrain(width - 3, height / 2 - 1, Tiles::Factory);

        addRoads(map.get());
        smoothTerrain(map.get());
        return map;
    }

} // namespace OpenWars::Game