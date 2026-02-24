#include "game.hpp"
#include "../map/map.hpp"
#include "../../io/graphics/graphics.hpp"
#include "../../io/log/logging.hpp"

using namespace OpenWars::IO::Graphics;
using namespace OpenWars::Drawing;

OpenWars::Game::GameScene::GameScene()
    : Scene("GameScene") {
    gameMap = std::make_unique<Map>(20, 20);
}

OpenWars::Game::GameScene::~GameScene() {
    unloadSpritesheets();
}

void OpenWars::Game::GameScene::loadSpritesheets() {
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

void OpenWars::Game::GameScene::unloadSpritesheets() {
    for(auto& [weather, sheet] : spritesheets) {
        delete sheet;
    }
    spritesheets.clear();
}

void OpenWars::Game::GameScene::initializeTileFrames() {
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

int OpenWars::Game::GameScene::getTerrainSpriteIndex(
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

int OpenWars::Game::GameScene::getTileFrameIndex(const TileFrame& frame) const {
    if(frame.frameCount <= 1) {
        return frame.spriteIndex;
    }

    int animFrame =
        static_cast<int>(frame.animationTime / ANIMATION_FRAME_TIME) %
        frame.frameCount;
    return frame.spriteIndex + animFrame;
}

int OpenWars::Game::GameScene::getAnimationFrameIndex(
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

void OpenWars::Game::GameScene::renderMap() {
    if(tileFrames.empty()) {
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

    int mapWidth = gameMap->getWidth();
    int mapHeight = gameMap->getHeight();

    for(int y = 0; y < mapHeight; ++y) {
        for(int x = 0; x < mapWidth; ++x) {
            const TileFrame& frame = tileFrames[y][x];
            int frameIndex = getTileFrameIndex(frame);

            float screenX = x * TILE_SIZE;
            float screenY = y * TILE_SIZE;

            sheet->drawFrame(frameIndex, screenX, screenY, 1.0f);
        }
    }
}

void OpenWars::Game::GameScene::onEnter() {
    Scene::onEnter();

    if(!initialized) {
        loadSpritesheets();

        gameMap->fillRectangle(0, 0, 10, 10, TerrainType::Plain, 0);
        gameMap->fillRectangle(10, 5, 5, 5, TerrainType::Mountain, 4);
        gameMap->fillRectangle(5, 12, 3, 3, TerrainType::Woods, 1);
        gameMap->fillRectangle(2, 15, 8, 2, TerrainType::River, 0);
        gameMap->fillRectangle(15, 0, 5, 10, TerrainType::Sea, 0);

        initializeTileFrames();
        initialized = true;
    }
}

void OpenWars::Game::GameScene::onExit() {
    Scene::onExit();
    unloadSpritesheets();
}

void OpenWars::Game::GameScene::update(float deltaTime) {
    Scene::update(deltaTime);

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

void OpenWars::Game::GameScene::render() {
    clearBackground(Color(50, 50, 50, 255));

    renderMap();

    Scene::render();
}

void OpenWars::Game::GameScene::setWeather(Weather weather) {
    if(currentWeather != weather) {
        currentWeather = weather;
        IO::Logging::debug("Weather changed to %d", static_cast<int>(weather));
    }
}
