#pragma once

#include "scene.hpp"
#include "../../core/drawing/spritesheet.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

namespace OpenWars::Game {
    class Map;
    enum class TerrainType;

    enum class Weather { Clear, Rain, Snow };

    class GameScene : public Scene {
      private:
        std::unique_ptr<Map> gameMap;
        std::unordered_map<int, Drawing::SpriteSheet*> spritesheets;

        struct TileFrame {
            int spriteIndex;
            float animationTime = 0.0f;
            float animationSpeed = 0.0f;
            int frameCount = 1;
        };
        std::vector<std::vector<TileFrame>> tileFrames;

        Weather currentWeather = Weather::Clear;
        float animationAccum = 0.0f;
        const float ANIMATION_FRAME_TIME = 1.f;
        const int TILE_SIZE = 16;
        int spritesheetCols = 0;

        void loadSpritesheets();
        void unloadSpritesheets();
        void initializeTileFrames();
        int getTerrainSpriteIndex(TerrainType type, int x, int y) const;
        int getTileFrameIndex(const TileFrame& frame) const;
        int getAnimationFrameIndex(TerrainType type, int animFrame) const;
        void renderMap();

      public:
        GameScene();
        ~GameScene() override;

        void onEnter() override;
        void onExit() override;
        void update(float deltaTime) override;
        void render() override;

        Map* getMap() const {
            return gameMap.get();
        }

        void setWeather(Weather weather);
        Weather getWeather() const {
            return currentWeather;
        }
    };
} // namespace OpenWars::Game
