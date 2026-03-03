#pragma once

#include "scene.hpp"
#include "../map/renderer.hpp"
#include "../../io/graphics/camera.hpp"
#include "../../io/input/input.hpp"
#include "../../core/vector.hpp"
#include <memory>

namespace OpenWars::Game {
    class Map;

    class GameScene : public Scene {
      private:
        // Game state
        std::unique_ptr<Map> gameMap;
        std::unique_ptr<IO::Graphics::Camera> camera;

        // Rendering and input subsystems
        std::unique_ptr<MapRenderer> mapRenderer;
        std::unique_ptr<IO::Graphics::CameraController> cameraController;

        IO::Input::InputState lastInputState;
        Vector2 cursorTile = {0, 0}; // Current tile cursor position

        // Cursor movement timing
        float cursorMoveDelay = 0.15f;  // Initial delay before repeat
        float cursorMoveRepeat = 0.05f; // Repeat rate after initial delay
        float cursorMoveTimer = 0.0f;
        bool cursorMoving = false; // Track if any arrow key is held

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
