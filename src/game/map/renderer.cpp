#include "renderer.hpp"
#include "../map/map.hpp"
#include "../../io/log/logging.hpp"

using namespace OpenWars::IO::Graphics;
using namespace OpenWars::Drawing;

OpenWars::Game::MapRenderer::MapRenderer(Map* map)
    : gameMap(map) {
}

OpenWars::Game::MapRenderer::~MapRenderer() {
    unloadSpritesheets();
}

void OpenWars::Game::MapRenderer::loadSpritesheets() {
    Drawing::SpriteSheet* sheet = Drawing::SpriteSheet::loadFromAssets(
        "sprites/terrain_clear.png",
        TILE_SIZE,
        TILE_SIZE
    );

    if(!sheet) {
        IO::Logging::error(
            false,
            "Failed to load spritesheet: %s",
            "sprites/terrain_clear.png"
        );
        return;
    }

    spritesheetCols = sheet->getCols();
    spritesheets[static_cast<int>(Weather::Clear)] = sheet;
}

void OpenWars::Game::MapRenderer::unloadSpritesheets() {
    for(auto& [weather, sheet] : spritesheets) {
        delete sheet;
    }
    spritesheets.clear();
}

void OpenWars::Game::MapRenderer::initializeTileFrames() {
    if(!gameMap) {
        return;
    }

    int width = gameMap->getWidth();
    int height = gameMap->getHeight();

    auto coord1Based = [this](int row, int col) {
        return (row - 1) * spritesheetCols + (col - 1);
    };

    tileFrames.resize(height);
    for(int y = 0; y < height; ++y) {
        tileFrames[y].resize(width);
        for(int x = 0; x < width; ++x) {
            TileFrame& frame = tileFrames[y][x];
            TerrainType terrainType = gameMap->getTerrain(x, y)->getType();

            frame.spriteIndex = getTerrainSpriteIndex(terrainType, x, y);

            if(terrainType == TerrainType::Sea) {
                frame.animationSpeed = 1.0f;
                frame.frameCount = 3;
                frame.spriteIndex = coord1Based(3, 1);
            } else if(terrainType == TerrainType::River) {
                frame.animationSpeed = 0.0f;
                frame.frameCount = 1;
                frame.spriteIndex = coord1Based(5, 3);
            }
        }
    }
}

int OpenWars::Game::MapRenderer::getTerrainSpriteIndex(
    TerrainType type,
    int x,
    int y
) const {
    auto coord1Based = [this](int row, int col) {
        return (row - 1) * spritesheetCols + (col - 1);
    };

    switch(type) {
    case TerrainType::Plain:
        return coord1Based(1, 1);
    case TerrainType::Woods:
        if(y % 2 == 0) {
            return coord1Based(1, 4);
        }
        return coord1Based(1, 3);
    case TerrainType::Mountain:
        return coord1Based(2, 5);
    case TerrainType::Sea:
        return coord1Based(3, 1);
    case TerrainType::Coast:
        return coord1Based(5, 1);
    case TerrainType::River:
        return coord1Based(5, 3);
    case TerrainType::Road:
        return coord1Based(1, 1);
    default:
        return coord1Based(1, 1);
    }
}

int OpenWars::Game::MapRenderer::getTileFrameIndex(
    const TileFrame& frame
) const {
    if(frame.frameCount <= 1) {
        return frame.spriteIndex;
    }

    int animFrame =
        static_cast<int>(frame.animationTime / ANIMATION_FRAME_TIME) %
        frame.frameCount;
    return frame.spriteIndex + animFrame;
}

int OpenWars::Game::MapRenderer::getAnimationFrameIndex(
    TerrainType type,
    int animFrame
) const {
    auto coord1Based = [this](int row, int col) {
        return (row - 1) * spritesheetCols + (col - 1);
    };

    switch(type) {
    case TerrainType::Sea:
        return coord1Based(3, 1 + animFrame);
    case TerrainType::River:
        return coord1Based(5 + animFrame, 3);
    default:
        return 0;
    }
}

OpenWars::Game::MapRenderer::TerrainLayer
OpenWars::Game::MapRenderer::getTerrainLayer(TerrainType type) const {
    // Foreground layer: elevated and visually prominent terrain
    switch(type) {
    case TerrainType::Woods:
    case TerrainType::Mountain:
    case TerrainType::HQ:
    case TerrainType::Factory:
    case TerrainType::Airport:
    case TerrainType::Port:
    case TerrainType::Lab:
    case TerrainType::CommTower:
    case TerrainType::Silo:
        return TerrainLayer::Foreground;

    // Background layer: plains and base terrain
    case TerrainType::Plain:
    case TerrainType::Road:
    case TerrainType::River:
    case TerrainType::Coast:
    case TerrainType::Sea:
    case TerrainType::City:
    case TerrainType::Pipe:
    default:
        return TerrainLayer::Background;
    }
}

void OpenWars::Game::MapRenderer::update(float deltaTime) {
    animationAccum += deltaTime;
    if(animationAccum >= ANIMATION_FRAME_TIME) {
        animationAccum -= ANIMATION_FRAME_TIME;

        for(auto& row : tileFrames) {
            for(auto& frame : row) {
                if(frame.animationSpeed > 0.0f) {
                    frame.animationTime += frame.animationSpeed;
                }
            }
        }
    }
}

void OpenWars::Game::MapRenderer::render(IO::Graphics::Camera* camera) {
    if(tileFrames.empty() || !camera) {
        return;
    }

    auto it = spritesheets.find(static_cast<int>(currentWeather));
    if(it == spritesheets.end()) {
        return;
    }

    SpriteSheet* sheet = it->second;
    if(!sheet) {
        return;
    }

    if(!gameMap) {
        return;
    }

    int mapWidth = gameMap->getWidth();
    int mapHeight = gameMap->getHeight();

    Vector3 cameraPos = camera->getPosition();
    float zoom = camera->getZoom();
    int viewportW = camera->getViewportWidth();
    int viewportH = camera->getViewportHeight();

    float scaledTileSize = TILE_SIZE * zoom;
    float camOffsetX = viewportW / 2.0f - (cameraPos.x * zoom);
    float camOffsetY = viewportH / 2.0f - (cameraPos.y * zoom);

    auto coord1Based = [this](int row, int col) {
        return (row - 1) * spritesheetCols + (col - 1);
    };

    // First pass: render plains as background for all tiles
    for(int y = 0; y < mapHeight; ++y) {
        for(int x = 0; x < mapWidth; ++x) {
            Terrain* terrain = gameMap->getTerrain(x, y);
            if(!terrain)
                continue;

            // For foreground terrain, render plains underneath
            if(getTerrainLayer(terrain->getType()) ==
               TerrainLayer::Foreground) {
                float screenX = (x * scaledTileSize) + camOffsetX;
                float screenY = (y * scaledTileSize) + camOffsetY;

                // Only draw if visible in viewport
                if(screenX + scaledTileSize > 0 && screenX < viewportW &&
                   screenY + scaledTileSize > 0 && screenY < viewportH) {
                    int plainSpriteIndex = coord1Based(1, 1);
                    sheet->drawFrame(plainSpriteIndex, screenX, screenY, zoom);
                }
            }
        }
    }

    // Second pass: render all terrain (both background and foreground)
    for(int y = 0; y < mapHeight; ++y) {
        for(int x = 0; x < mapWidth; ++x) {
            Terrain* terrain = gameMap->getTerrain(x, y);
            if(!terrain)
                continue;

            const TileFrame& frame = tileFrames[y][x];
            int frameIndex = getTileFrameIndex(frame);

            float screenX = (x * scaledTileSize) + camOffsetX;
            float screenY = (y * scaledTileSize) + camOffsetY;

            // Only draw if visible in viewport
            if(screenX + scaledTileSize > 0 && screenX < viewportW &&
               screenY + scaledTileSize > 0 && screenY < viewportH) {
                sheet->drawFrame(frameIndex, screenX, screenY, zoom);
            }
        }
    }
}

void OpenWars::Game::MapRenderer::setWeather(Weather weather) {
    if(currentWeather != weather) {
        currentWeather = weather;
        IO::Logging::debug("Weather changed to %d", static_cast<int>(weather));
    }
}
