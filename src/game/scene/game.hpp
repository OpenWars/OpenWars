#pragma once

#include "scene.hpp"
#include "../map/renderer.hpp"
#include "../units/renderer.hpp"
#include "../../io/graphics/camera.hpp"
#include "../../io/input/input.hpp"
#include "../../core/vector.hpp"
#include <memory>

namespace OpenWars::Game {
    class Map;

    class GameScene : public Scene {
        private:
        std::unique_ptr<Map> gameMap;
        std::unique_ptr<IO::Graphics::Camera> camera;

        std::unique_ptr<MapRenderer> mapRenderer;
        std::unique_ptr<UnitRenderer> unitRenderer;
        std::unique_ptr<IO::Graphics::CameraController> cameraController;

        IO::Input::InputState lastInputState;
        Vector2 cursorTile = {0, 0};

        float cursorMoveDelay = 0.15f;
        float cursorMoveRepeat = 0.05f;
        float cursorMoveTimer = 0.0f;
        bool cursorMoving = false;

        void initializeCamera();

        public:
        GameScene();
        ~GameScene() override;

        void onEnter() override;
        void onExit() override;
        void update(float deltaTime) override;
        void render() override;
        void handleInput(const IO::Input::InputState& input) override;

        Map* getMap() const {
            return gameMap.get();
        }

        IO::Graphics::Camera* getCamera() const {
            return camera.get();
        }

        void setWeather(Weather weather);
        Weather getWeather() const {
            return mapRenderer ? mapRenderer->getWeather() : Weather::Clear;
        }
    };
} // namespace OpenWars::Game