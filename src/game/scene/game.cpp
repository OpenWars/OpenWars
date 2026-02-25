#include "game.hpp"
#include "../map/map.hpp"
#include "../../io/graphics/graphics.hpp"
#include "../../io/input/input.hpp"
#include "../../io/log/logging.hpp"

using namespace OpenWars::IO::Graphics;
using namespace OpenWars::Drawing;

OpenWars::Game::GameScene::GameScene()
    : Scene("GameScene") {
    gameMap = std::make_unique<Map>(20, 20);
    camera = std::make_unique<IO::Graphics::Camera>();
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

void OpenWars::Game::GameScene::initializeCamera() {
    int mapWidth = gameMap->getWidth();
    int mapHeight = gameMap->getHeight();

    float mapPixelWidth = mapWidth * TILE_SIZE;
    float mapPixelHeight = mapHeight * TILE_SIZE;

    float mapCenterX = mapPixelWidth / 2.0f;
    float mapCenterY = mapPixelHeight / 2.0f;

    camera->instantPan(Vector3{mapCenterX, mapCenterY, 10.0f});
    camera->setBoundaries(0.0f, mapPixelWidth, 0.0f, mapPixelHeight);
    camera->setZoomLimits(0.5f, 4.0f);
    camera->setZoom(1.0f);
}

void OpenWars::Game::GameScene::handleCameraInput(
    const IO::Input::InputState& input
) {
    if(!camera)
        return;

    const float panSpeed = 300.0f;
    const float zoomSpeed = 0.1f;

    float panX = 0.0f;
    float panY = 0.0f;

    if(input.down.arrowLeft || input.down.A) {
        panX = -panSpeed;
    }
    if(input.down.arrowRight || input.down.D) {
        panX = panSpeed;
    }
    if(input.down.arrowUp || input.down.W) {
        panY = -panSpeed;
    }
    if(input.down.arrowDown || input.down.S) {
        panY = panSpeed;
    }

    if(panX != 0.0f || panY != 0.0f) {
        camera->pan(
            panX * IO::Graphics::getFrameTime(),
            panY * IO::Graphics::getFrameTime()
        );
    }

    if(input.pressed.rightClick) {
        camera->applyZoom(zoomSpeed);
    }
    if(input.pressed.leftClick) {
        camera->applyZoom(-zoomSpeed);
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

    Vector3 cameraPos = camera->getPosition();
    float zoom = camera->getZoom();
    int viewportW = camera->getViewportWidth();
    int viewportH = camera->getViewportHeight();

    float scaledTileSize = TILE_SIZE * zoom;
    float camOffsetX = viewportW / 2.0f - (cameraPos.x * zoom);
    float camOffsetY = viewportH / 2.0f - (cameraPos.y * zoom);

    for(int y = 0; y < mapHeight; ++y) {
        for(int x = 0; x < mapWidth; ++x) {
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
        initializeCamera();
        initialized = true;
    }
}

void OpenWars::Game::GameScene::onExit() {
    Scene::onExit();
    unloadSpritesheets();
}

void OpenWars::Game::GameScene::update(float deltaTime) {
    Scene::update(deltaTime);

    // Update camera with input
    handleCameraInput(lastInputState);

    // Update camera
    if(camera) {
        camera->update(deltaTime);
    }

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

void OpenWars::Game::GameScene::handleInput(
    const IO::Input::InputState& input
) {
    lastInputState = input;
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
