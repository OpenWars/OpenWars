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
    SpriteSheet* sheet = SpriteSheet::loadFromAssets(
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
    for(auto& [weather, sheet] : spritesheets)
        delete sheet;
    spritesheets.clear();
}

void OpenWars::Game::MapRenderer::initializeTileFrames() {
    if(!gameMap)
        return;

    const int width = gameMap->getWidth();
    const int height = gameMap->getHeight();
    tileFrameWidth = width;

    tileFrames.resize(width * height);
    animatedTileIndices.clear();

    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            const int flat = y * width + x;
            TileFrame& frame = tileFrames[flat];
            const TerrainType type = gameMap->getTerrain(x, y)->getType();

            frame.spriteIndex = getTerrainSpriteIndex(type, x, y);

            if(type == TerrainType::Sea) {
                frame.animationSpeed = 1.0f;
                frame.frameCount = 3;
                frame.spriteIndex = coord1Based(3, 1);
            } else if(type == TerrainType::River) {
                frame.animationSpeed = 0.0f;
                frame.frameCount = 1;
                frame.spriteIndex = coord1Based(5, 3);
            }

            // Track only the tiles that actually animate.
            if(frame.animationSpeed > 0.0f)
                animatedTileIndices.push_back(flat);
        }
    }
}

int OpenWars::Game::MapRenderer::getTerrainSpriteIndex(
    TerrainType type,
    int x,
    int y
) const {
    switch(type) {
    case TerrainType::Plain:
        return coord1Based(1, 1);
    case TerrainType::Woods:
        return (y % 2 == 0) ? coord1Based(1, 4) : coord1Based(1, 3);
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
    if(frame.frameCount <= 1)
        return frame.spriteIndex;
    int animFrame =
        static_cast<int>(frame.animationTime / ANIMATION_FRAME_TIME) %
        frame.frameCount;
    return frame.spriteIndex + animFrame;
}

int OpenWars::Game::MapRenderer::getAnimationFrameIndex(
    TerrainType type,
    int animFrame
) const {
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
    default:
        return TerrainLayer::Background;
    }
}

void OpenWars::Game::MapRenderer::update(float deltaTime) {
    if(animatedTileIndices.empty())
        return;

    animationAccum += deltaTime;
    if(animationAccum < ANIMATION_FRAME_TIME)
        return;
    animationAccum -= ANIMATION_FRAME_TIME;

    for(int flat : animatedTileIndices)
        tileFrames[flat].animationTime += tileFrames[flat].animationSpeed;
}

OpenWars::Vector2 OpenWars::Game::MapRenderer::getTileAtScreenPos(
    const Vector2& screenPos,
    IO::Graphics::Camera* camera
) const {
    if(!camera || !gameMap)
        return Vector2{-1, -1};

    Vector3 cameraPos = camera->getPosition();
    float zoom = camera->getZoom();
    int viewportW = camera->getViewportWidth();
    int viewportH = camera->getViewportHeight();

    float scaledTileSize = TILE_SIZE * zoom;
    float camOffsetX = viewportW / 2.0f - (cameraPos.x * zoom);
    float camOffsetY = viewportH / 2.0f - (cameraPos.y * zoom);

    int tileX = (int)((screenPos.x - camOffsetX) / scaledTileSize);
    int tileY = (int)((screenPos.y - camOffsetY) / scaledTileSize);

    if(gameMap->isInBounds(tileX, tileY))
        return Vector2{(float)tileX, (float)tileY};
    return Vector2{-1, -1};
}

void OpenWars::Game::MapRenderer::render(IO::Graphics::Camera* camera) {
    if(tileFrames.empty() || !camera || !gameMap)
        return;

    auto it = spritesheets.find(static_cast<int>(currentWeather));
    if(it == spritesheets.end())
        return;

    SpriteSheet* sheet = it->second;
    if(!sheet)
        return;

    const int mapWidth = gameMap->getWidth();
    const int mapHeight = gameMap->getHeight();

    Vector3 cameraPos = camera->getPosition();
    float zoom = camera->getZoom();
    int viewportW = camera->getViewportWidth();
    int viewportH = camera->getViewportHeight();

    float scaledTileSize = TILE_SIZE * zoom;
    float camOffsetX = viewportW / 2.0f - (cameraPos.x * zoom);
    float camOffsetY = viewportH / 2.0f - (cameraPos.y * zoom);

    const int plainIdx = coord1Based(1, 1);

    // Pass 1: plain underlay for foreground terrain
    for(int y = 0; y < mapHeight; ++y) {
        for(int x = 0; x < mapWidth; ++x) {
            Terrain* terrain = gameMap->getTerrain(x, y);
            if(!terrain)
                continue;
            if(getTerrainLayer(terrain->getType()) != TerrainLayer::Foreground)
                continue;

            float screenX = (x * scaledTileSize) + camOffsetX;
            float screenY = (y * scaledTileSize) + camOffsetY;
            if(screenX + scaledTileSize <= 0 || screenX >= viewportW)
                continue;
            if(screenY + scaledTileSize <= 0 || screenY >= viewportH)
                continue;

            sheet->drawFrame(plainIdx, screenX, screenY, zoom);
        }
    }

    // Pass 2: all terrain
    for(int y = 0; y < mapHeight; ++y) {
        for(int x = 0; x < mapWidth; ++x) {
            float screenX = (x * scaledTileSize) + camOffsetX;
            float screenY = (y * scaledTileSize) + camOffsetY;
            if(screenX + scaledTileSize <= 0 || screenX >= viewportW)
                continue;
            if(screenY + scaledTileSize <= 0 || screenY >= viewportH)
                continue;

            const int flat = y * tileFrameWidth + x;
            int frameIndex = getTileFrameIndex(tileFrames[flat]);
            sheet->drawFrame(frameIndex, screenX, screenY, zoom);
        }
    }
}

void OpenWars::Game::MapRenderer::setWeather(Weather weather) {
    if(currentWeather != weather) {
        currentWeather = weather;
        IO::Logging::debug("Weather changed to %d", static_cast<int>(weather));
    }
}