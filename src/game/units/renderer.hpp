#pragma once

#include "../../core/drawing/spritesheet.hpp"
#include "../../io/graphics/camera.hpp"
#include "../map/renderer.hpp"
#include <unordered_map>

namespace OpenWars::Game {
    class Map;

    class UnitRenderer {
        private:
        std::unordered_map<int, Drawing::SpriteSheet*> spritesheets;
        int spritesheetCols = 0;
        float animationAccum = 0.0f;
        int animationFrame = 0;
        const float ANIMATION_FRAME_TIME = 0.5f;
        const int TILE_SIZE = 16;
        Weather currentWeather = Weather::Clear;

        int coord1Based(int row, int col) const {
            return (row - 1) * spritesheetCols + (col - 1);
        }

        public:
        UnitRenderer();
        ~UnitRenderer();

        void loadSpritesheets();
        void update(float deltaTime);
        void render(const Map* map, IO::Graphics::Camera* camera);
        void setWeather(Weather weather);
    };
} // namespace OpenWars::Game