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
    gameMap = MapGenerator::generateTestMap(20, 20);
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
    camera->setZoom(2.0f); // Start at 2.5x for good visibility
}

void OpenWars::Game::GameScene::onEnter() {
    Scene::onEnter();

    if(!initialized) {
        // Initialize rendering subsystem
        mapRenderer = std::make_unique<MapRenderer>(gameMap.get());
        mapRenderer->loadSpritesheets();
        mapRenderer->initializeTileFrames();

        // Initialize camera system
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

    // Update camera with input
    if(cameraController) {
        cameraController->handleInput(lastInputState);
    }

    // Update camera
    if(camera) {
        camera->update(deltaTime);
    }

    // Update map rendering (animations)
    if(mapRenderer) {
        mapRenderer->update(deltaTime);
    }

    // Handle continuous cursor movement with key repeat
    bool anyArrowPressed =
        lastInputState.down.arrowLeft || lastInputState.down.arrowRight ||
        lastInputState.down.arrowUp || lastInputState.down.arrowDown;

    if(anyArrowPressed) {
        if(!cursorMoving) {
            // Key just pressed, start the timer
            cursorMoving = true;
            cursorMoveTimer = 0.0f;
        }

        // Accumulate time
        cursorMoveTimer += deltaTime;

        // Check if we should move the cursor
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
            if(lastInputState.down.arrowLeft) {
                movement.x--;
            }
            if(lastInputState.down.arrowRight) {
                movement.x++;
            }
            if(lastInputState.down.arrowUp) {
                movement.y--;
            }
            if(lastInputState.down.arrowDown) {
                movement.y++;
            }

            cursorTile += movement;

            // Clamp cursor to map bounds
            if(gameMap) {
                int mapWidth = gameMap->getWidth();
                int mapHeight = gameMap->getHeight();
                cursorTile.x = std::clamp((int)cursorTile.x, 0, mapWidth - 1);
                cursorTile.y = std::clamp((int)cursorTile.y, 0, mapHeight - 1);
            }
        }
    } else {
        // No arrow key pressed, reset state
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

    // Move cursor immediately on first key press (handled in update for repeat)
    if(input.pressed.arrowLeft || input.pressed.arrowRight ||
       input.pressed.arrowUp || input.pressed.arrowDown) {
        Vector2 movement = {0, 0};
        if(input.pressed.arrowLeft) {
            movement.x--;
        }
        if(input.pressed.arrowRight) {
            movement.x++;
        }
        if(input.pressed.arrowUp) {
            movement.y--;
        }
        if(input.pressed.arrowDown) {
            movement.y++;
        }

        cursorTile += movement;

        // Clamp cursor to map bounds
        if(gameMap) {
            int mapWidth = gameMap->getWidth();
            int mapHeight = gameMap->getHeight();
            cursorTile.x = std::clamp((int)cursorTile.x, 0, mapWidth - 1);
            cursorTile.y = std::clamp((int)cursorTile.y, 0, mapHeight - 1);
        }

        // Reset the repeat timer so holding key starts fresh
        cursorMoveTimer = 0.0f;
        cursorMoving = true;
    }
}

void OpenWars::Game::GameScene::render() {
    clearBackground(Color(50, 50, 50, 255));

    // Render map through dedicated system
    if(mapRenderer) {
        mapRenderer->render(camera.get());
    }

    // Render tile cursor overlay
    if(mapRenderer && camera) {
        // Use cursor tile position
        Vector2 tilePos = cursorTile;
        if(tilePos.x >= 0 && tilePos.y >= 0) {
            // Calculate screen position of the tile
            Vector3 cameraPos = camera->getPosition();
            float zoom = camera->getZoom();
            int viewportW = camera->getViewportWidth();
            int viewportH = camera->getViewportHeight();
            const int TILE_SIZE = 16;

            float scaledTileSize = TILE_SIZE * zoom;
            float camOffsetX = viewportW / 2.0f - (cameraPos.x * zoom);
            float camOffsetY = viewportH / 2.0f - (cameraPos.y * zoom);

            float screenX = ((int)tilePos.x * scaledTileSize) + camOffsetX;
            float screenY = ((int)tilePos.y * scaledTileSize) + camOffsetY;

            // Draw cursor overlay on the tile
            Drawing::drawRectangle(
                (int)screenX,
                (int)screenY,
                (int)scaledTileSize,
                (int)scaledTileSize,
                Color(255, 255, 0, 100)
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