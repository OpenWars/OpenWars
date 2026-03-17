#include "mapgen.hpp"
#include <random>
#include <cmath>
#include <algorithm>

namespace OpenWars::Game {

    static std::mt19937 generator(42);
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    float MapGenerator::perlinNoise(int x, int y, int seed) {
        // Simple pseudo-random function based on coordinates
        int n = std::sin(x * 12.9898f + y * 78.233f + seed) * 43758.5453f;
        return (float)(n - std::floor(n));
    }

    float MapGenerator::smoothstep(float t) {
        // Smoothstep interpolation: 3t^2 - 2t^3
        return t * t * (3.0f - 2.0f * t);
    }

    void MapGenerator::smoothTerrain(Map* map, int radius) {
        // Create a copy to avoid modifying while reading
        auto width = map->getWidth();
        auto height = map->getHeight();
        std::vector<std::vector<TerrainType>> smoothed(
            height,
            std::vector<TerrainType>(width)
        );

        map->forEachTile([&](int x, int y, Terrain* t) {
            int waterCount = 0;
            int landCount = 0;

            // Check neighbors
            for(int dy = -radius; dy <= radius; ++dy) {
                for(int dx = -radius; dx <= radius; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if(map->isInBounds(nx, ny)) {
                        auto neighbor = map->getTerrain(nx, ny);
                        if(neighbor) {
                            auto type = neighbor->getType();
                            if(type == TerrainType::Sea ||
                               type == TerrainType::River) {
                                waterCount++;
                            } else if(type != TerrainType::Mountain) {
                                landCount++;
                            }
                        }
                    }
                }
            }

            // Smooth transitions
            auto current = t->getType();
            if((current == TerrainType::Plain ||
                current == TerrainType::Woods) &&
               waterCount > landCount) {
                smoothed[y][x] = TerrainType::Coast;
            } else {
                smoothed[y][x] = current;
            }
        });

        // Apply smoothed terrain
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                map->setTerrain(x, y, smoothed[y][x], 0);
            }
        }
    }

    void MapGenerator::addStructures(Map* map, int count, TerrainType type) {
        std::uniform_int_distribution<> widthDist(0, map->getWidth() - 1);
        std::uniform_int_distribution<> heightDist(0, map->getHeight() - 1);

        for(int i = 0; i < count; ++i) {
            int x = widthDist(generator);
            int y = heightDist(generator);
            auto terrain = map->getTerrain(x, y);
            if(terrain && terrain->getType() == TerrainType::Plain) {
                map->setTerrain(x, y, type, 2);
            }
        }
    }

    void MapGenerator::addRoads(Map* map) {
        // Add some roads in a grid pattern
        for(int y = 5; y < map->getHeight(); y += 8) {
            for(int x = 0; x < map->getWidth(); ++x) {
                auto terrain = map->getTerrain(x, y);
                if(terrain && terrain->getType() == TerrainType::Plain) {
                    map->setTerrain(x, y, TerrainType::Road, 0);
                }
            }
        }
    }

    std::unique_ptr<Map> MapGenerator::generateRandom(int width, int height) {
        auto map = std::make_unique<Map>(width, height);

        std::uniform_real_distribution<> terrainDist(0.0, 1.0);

        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                float noise = terrainDist(generator);

                TerrainType type;
                if(noise < 0.3f) {
                    type = TerrainType::Sea;
                } else if(noise < 0.4f) {
                    type = TerrainType::Coast;
                } else if(noise < 0.65f) {
                    type = TerrainType::Plain;
                } else if(noise < 0.85f) {
                    type = TerrainType::Woods;
                } else {
                    type = TerrainType::Mountain;
                }

                int defense = type == TerrainType::Mountain   ? 4
                              : type == TerrainType::Woods    ? 2
                              : type == TerrainType::Mountain ? 1
                                                              : 0;
                map->setTerrain(x, y, type, defense);
            }
        }

        smoothTerrain(map.get());
        return map;
    }

    std::unique_ptr<Map>
    MapGenerator::generateIslandMap(int width, int height, int islandCount) {
        auto map = std::make_unique<Map>(width, height);

        // Fill with sea
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                map->setTerrain(x, y, TerrainType::Sea, 0);
            }
        }

        // Generate islands
        std::uniform_int_distribution<> widthDist(5, width - 5);
        std::uniform_int_distribution<> heightDist(5, height - 5);
        std::uniform_int_distribution<> sizeDist(3, 8);

        for(int i = 0; i < islandCount; ++i) {
            int cx = widthDist(generator);
            int cy = heightDist(generator);
            int size = sizeDist(generator);

            // Create island with circular shape
            for(int y = cy - size; y <= cy + size; ++y) {
                for(int x = cx - size; x <= cx + size; ++x) {
                    if(map->isInBounds(x, y)) {
                        int dx = x - cx;
                        int dy = y - cy;
                        if(dx * dx + dy * dy <= size * size) {
                            map->setTerrain(x, y, TerrainType::Plain, 0);
                        }
                    }
                }
            }
        }

        smoothTerrain(map.get());
        addStructures(
            map.get(),
            std::max(2, islandCount / 2),
            TerrainType::City
        );
        return map;
    }

    std::unique_ptr<Map> MapGenerator::generateRiverMap(int width, int height) {
        auto map = std::make_unique<Map>(width, height);

        // Fill with plain
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                map->setTerrain(x, y, TerrainType::Plain, 0);
            }
        }

        // Create rivers flowing from top to bottom
        int numRivers = 2;
        for(int r = 0; r < numRivers; ++r) {
            int x = (r + 1) * width / (numRivers + 1);
            for(int y = 0; y < height; ++y) {
                if(map->isInBounds(x, y)) {
                    map->setTerrain(x, y, TerrainType::River, 0);
                }
                // Meander slightly
                if(distribution(generator) < 0.3f) {
                    x += distribution(generator) < 0.5f ? 1 : -1;
                }
            }
        }

        // Add some forests
        for(int i = 0; i < 15; ++i) {
            std::uniform_int_distribution<> widthDist(2, width - 3);
            std::uniform_int_distribution<> heightDist(2, height - 3);
            int x = widthDist(generator);
            int y = heightDist(generator);
            map->fillRectangle(x, y, 3, 3, TerrainType::Woods, 2);
        }

        addStructures(map.get(), 4, TerrainType::City);
        return map;
    }

    std::unique_ptr<Map>
    MapGenerator::generateMountainMap(int width, int height) {
        auto map = std::make_unique<Map>(width, height);

        // Generate base terrain with Perlin-like noise
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                float noise = perlinNoise(x / 4, y / 4, 12) * 0.5f +
                              perlinNoise(x / 8, y / 8, 42) * 0.3f +
                              perlinNoise(x / 16, y / 16, 99) * 0.2f;

                TerrainType type;
                if(noise < 0.2f) {
                    type = TerrainType::Mountain;
                } else if(noise < 0.4f) {
                    type = TerrainType::Woods;
                } else if(noise < 0.6f) {
                    type = TerrainType::Plain;
                } else {
                    type = TerrainType::Road;
                }

                int defense = type == TerrainType::Mountain ? 4 : 1;
                map->setTerrain(x, y, type, defense);
            }
        }

        smoothTerrain(map.get());
        addRoads(map.get());
        return map;
    }

    std::unique_ptr<Map> MapGenerator::generateTestMap(int width, int height) {
        auto map = std::make_unique<Map>(width, height);

        // Fill base with Plain
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                map->setTerrain(x, y, TerrainType::Plain, 0);
            }
        }

        // Top-left: Mountains
        map->fillRectangle(0, 0, 4, 4, TerrainType::Mountain, 4);
        map->fillRectangle(0, 0, 4, 2, TerrainType::HighMountain, 4);

        // Top-right: Forest
        map->fillRectangle(width - 4, 0, 4, 4, TerrainType::Woods, 2);

        // Bottom-left: Sea
        map->fillRectangle(0, height - 4, 4, 4, TerrainType::Sea, 0);

        // Coast bordering the sea (one row/col above and right of sea block)
        for(int x = 0; x < 4; ++x) {
            map->setTerrain(x, height - 5, TerrainType::Coast, 0);
        }

        for(int y = height - 4; y < height; ++y) {
            map->setTerrain(4, y, TerrainType::Coast, 0);
        }

        // coast on corners too
        map->setTerrain(4, height - 5, TerrainType::Coast, 0);
        map->setTerrain(4, height - 4, TerrainType::Coast, 0);

        // River flowing vertically near left-center
        for(int y = 0; y < height - 4; ++y) {
            map->setTerrain(width / 4, y, TerrainType::River, 0);
        }

        // Horizontal road across center
        for(int x = 0; x < width; ++x) {
            map->setTerrain(x, height / 2, TerrainType::Road, 0);
        }

        // Vertical road across center
        for(int y = 0; y < height; ++y) {
            map->setTerrain(width / 2, y, TerrainType::Road, 0);
        }

        // Pipe segment along top edge
        for(int x = 4; x < width / 2 - 1; ++x) {
            map->setTerrain(x, 1, TerrainType::Pipe, 0);
        }

        // Player 1 side (left)
        map->setTerrain(1, 1, TerrainType::HQ, 4);
        map->setTerrain(3, height / 2 - 3, TerrainType::Factory, 3);
        map->setTerrain(5, height / 2 - 3, TerrainType::City, 2);
        map->setTerrain(3, height / 2 + 3, TerrainType::Airport, 2);
        map->setTerrain(5, height / 2 + 3, TerrainType::Port, 1);

        // Player 2 side (right)
        map->setTerrain(width - 2, height - 2, TerrainType::HQ, 4);
        map->setTerrain(width - 4, height / 2 - 3, TerrainType::Factory, 3);
        map->setTerrain(width - 6, height / 2 - 3, TerrainType::City, 2);
        map->setTerrain(width - 4, height / 2 + 3, TerrainType::Airport, 2);
        map->setTerrain(width - 6, height / 2 + 3, TerrainType::Port, 1);

        // Neutral structures in center quadrants
        map->setTerrain(
            width / 2 - 3,
            height / 2 - 3,
            TerrainType::CommTower,
            1
        );
        map->setTerrain(
            width / 2 + 3,
            height / 2 - 3,
            TerrainType::CommTower,
            1
        );
        map->setTerrain(width / 2 - 3, height / 2 + 3, TerrainType::Lab, 1);
        map->setTerrain(width / 2 + 3, height / 2 + 3, TerrainType::Silo, 0);

        return map;
    }

    std::unique_ptr<Map>
    MapGenerator::generateSymmetricalMap(int width, int height) {
        auto map = std::make_unique<Map>(width, height);

        // Generate left side
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width / 2; ++x) {
                float noise = perlinNoise(x, y, 15);

                TerrainType type;
                if(noise < 0.3f) {
                    type = TerrainType::Mountain;
                } else if(noise < 0.5f) {
                    type = TerrainType::Woods;
                } else {
                    type = TerrainType::Plain;
                }

                int defense = type == TerrainType::Mountain ? 4 : 1;
                map->setTerrain(x, y, type, defense);
            }
        }

        // Mirror to right side for symmetry
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width / 2; ++x) {
                auto terrain = map->getTerrain(width / 2 - 1 - x, y);
                if(terrain) {
                    map->setTerrain(
                        width / 2 + x,
                        y,
                        terrain->getType(),
                        terrain->getDefenseStars()
                    );
                }
            }
        }

        // Add bases at opposite corners
        map->setTerrain(2, height / 2 - 1, TerrainType::Factory, 3);
        map->setTerrain(width - 3, height / 2 - 1, TerrainType::Factory, 3);

        addRoads(map.get());
        smoothTerrain(map.get());
        return map;
    }

} // namespace OpenWars::Game
