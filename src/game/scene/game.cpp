#include "game.hpp"
#include "../map/map.hpp"
#include "../map/mapgen.hpp"
#include "../../io/graphics/graphics.hpp"
#include "../../io/input/input.hpp"
#include "../../io/log/logging.hpp"
#include <algorithm>
#include <cmath>

using namespace OpenWars::IO::Graphics;
using namespace OpenWars::Drawing;

OpenWars::Game::GameScene::GameScene()
    : Scene("GameScene") {
    gameMap = MapGenerator::generateTestMap(40, 40);
    camera = std::make_unique<IO::Graphics::Camera>();
}

OpenWars::Game::GameScene::~GameScene() {
}

void OpenWars::Game::GameScene::initializeCamera() {
    const int TILE_SIZE = 16;

    int mapWidth = gameMap->getWidth();
    int mapHeight = gameMap->getHeight();

    float mapPixelWidth = mapWidth * TILE_SIZE;
    float mapPixelHeight = mapHeight * TILE_SIZE;

    float mapCenterX = mapPixelWidth / 2.0f;
    float mapCenterY = mapPixelHeight / 2.0f;

    camera->instantPan(Vector3{mapCenterX, mapCenterY, 10.0f});
    camera->setBoundaries(0.0f, mapPixelWidth, 0.0f, mapPixelHeight);

    camera->setZoomLimits(1.0f, 3.5f);
    camera->setZoom(2.0f);
}

void OpenWars::Game::GameScene::onEnter() {
    Scene::onEnter();

    if(!initialized) {
        mapRenderer = std::make_unique<MapRenderer>(gameMap.get());
        mapRenderer->loadSpritesheets();
        mapRenderer->initializeTileFrames();

        initializeCamera();
        cameraController = std::make_unique<CameraController>(camera.get());

        initialized = true;
    }
}

void OpenWars::Game::GameScene::onExit() {
    Scene::onExit();
    mapRenderer.reset();
    cameraController.reset();
}

void OpenWars::Game::GameScene::update(float deltaTime) {
    Scene::update(deltaTime);

    if(cameraController) {
        cameraController->handleInput(lastInputState);
    }

    if(camera) {
        camera->update(deltaTime);
    }

    if(mapRenderer) {
        mapRenderer->update(deltaTime);
    }

    bool anyArrowPressed =
        lastInputState.down.arrowLeft || lastInputState.down.arrowRight ||
        lastInputState.down.arrowUp || lastInputState.down.arrowDown;

    if(anyArrowPressed) {
        if(!cursorMoving) {
            cursorMoving = true;
            cursorMoveTimer = 0.0f;
        }

        cursorMoveTimer += deltaTime;

        bool shouldMove = false;
        if(cursorMoveTimer >= cursorMoveDelay) {
            // After initial delay, move every repeat interval
            float timeAfterDelay = cursorMoveTimer - cursorMoveDelay;
            if(timeAfterDelay >= cursorMoveRepeat) {
                shouldMove = true;
                cursorMoveTimer =
                    cursorMoveDelay + fmod(timeAfterDelay, cursorMoveRepeat);
            }
        }

        if(shouldMove) {
            Vector2 movement = {0, 0};
            if(lastInputState.down.arrowLeft)
                movement.x--;
            if(lastInputState.down.arrowRight)
                movement.x++;
            if(lastInputState.down.arrowUp)
                movement.y--;
            if(lastInputState.down.arrowDown)
                movement.y++;

            cursorTile += movement;

            if(gameMap) {
                int mapWidth = gameMap->getWidth();
                int mapHeight = gameMap->getHeight();
                cursorTile.x = std::clamp((int)cursorTile.x, 0, mapWidth - 1);
                cursorTile.y = std::clamp((int)cursorTile.y, 0, mapHeight - 1);
            }
        }
    } else {
        cursorMoving = false;
        cursorMoveTimer = 0.0f;
    }

    if(camera) {
        const int TILE_SIZE = 16;
        float cursorWorldX = (cursorTile.x + 0.5f) * TILE_SIZE;
        float cursorWorldY = (cursorTile.y + 0.5f) * TILE_SIZE;

        camera->panTo(Vector3{cursorWorldX, cursorWorldY, 10.0f}, 0.3f);
    }
}

void OpenWars::Game::GameScene::handleInput(
    const IO::Input::InputState& input
) {
    lastInputState = input;

    if(input.pressed.arrowLeft || input.pressed.arrowRight ||
       input.pressed.arrowUp || input.pressed.arrowDown) {
        Vector2 movement = {0, 0};
        if(input.pressed.arrowLeft)
            movement.x--;
        if(input.pressed.arrowRight)
            movement.x++;
        if(input.pressed.arrowUp)
            movement.y--;
        if(input.pressed.arrowDown)
            movement.y++;

        cursorTile += movement;

        if(gameMap) {
            int mapWidth = gameMap->getWidth();
            int mapHeight = gameMap->getHeight();
            cursorTile.x = std::clamp((int)cursorTile.x, 0, mapWidth - 1);
            cursorTile.y = std::clamp((int)cursorTile.y, 0, mapHeight - 1);
        }

        cursorMoveTimer = 0.0f;
        cursorMoving = true;
    }
}

void OpenWars::Game::GameScene::render() {
    clearBackground(Color(50, 50, 50, 255));

    if(mapRenderer) {
        mapRenderer->render(camera.get());
    }

    if(mapRenderer && camera) {
        Vector2 tilePos = cursorTile;
        if(tilePos.x >= 0 && tilePos.y >= 0) {
            const int TILE_SIZE = 16;
            float scaledTileSize = TILE_SIZE * camera->getZoom();

            // Use the camera matrix pipeline so the cursor stays aligned with
            // the tile renderer under any camera transformation.
            Vector2 tileScreen = camera->worldToScreen(
                Vector3{tilePos.x * TILE_SIZE, tilePos.y * TILE_SIZE, 0.0f}
            );

            Drawing::drawRectangle(
                tileScreen.x,
                tileScreen.y,
                scaledTileSize,
                scaledTileSize,
                Colors::alpha(Colors::SKY_400, 0.4f)
            );

            Drawing::drawRectangleOutline(
                tileScreen.x,
                tileScreen.y,
                scaledTileSize,
                scaledTileSize,
                Colors::alpha(Colors::SKY_400, 0.4f)
            );
        }
    }

    Scene::render();
}

void OpenWars::Game::GameScene::setWeather(Weather weather) {
    if(mapRenderer) {
        mapRenderer->setWeather(weather);
        IO::Logging::debug("Weather changed to %d", static_cast<int>(weather));
    }
}