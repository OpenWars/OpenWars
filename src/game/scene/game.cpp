#include "game.hpp"
#include "../map/map.hpp"
#include "../map/mapgen.hpp"
#include "../../io/graphics/graphics.hpp"
#include "../../io/input/input.hpp"
#include "../../io/log/logging.hpp"

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
    camera->setZoomLimits(0.5f, 4.0f);
    camera->setZoom(1.0f);
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
}

void OpenWars::Game::GameScene::handleInput(
    const IO::Input::InputState& input
) {
    lastInputState = input;
}

void OpenWars::Game::GameScene::render() {
    clearBackground(Color(50, 50, 50, 255));

    // Render map through dedicated system
    if(mapRenderer) {
        mapRenderer->render(camera.get());
    }

    Scene::render();
}

void OpenWars::Game::GameScene::setWeather(Weather weather) {
    if(mapRenderer) {
        mapRenderer->setWeather(weather);
        IO::Logging::debug("Weather changed to %d", static_cast<int>(weather));
    }
}
