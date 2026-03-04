#pragma once

#include "../../core/drawing/spritesheet.hpp"
#include "../../core/vector.hpp"
#include "../../io/graphics/camera.hpp"
#include <unordered_map>
#include <vector>

namespace OpenWars::Game {
    class Map;
    enum class TerrainType;
    enum class Weather { Clear, Rain, Snow };

    class MapRenderer {
      private:
        struct TileFrame {
            int spriteIndex = 0;
            float animationTime = 0.0f;
            float animationSpeed = 0.0f;
            int frameCount = 1;
        };

        enum class TerrainLayer { Background, Foreground };

        Map* gameMap;
        std::unordered_map<int, Drawing::SpriteSheet*> spritesheets;

        std::vector<TileFrame> tileFrames;
        int tileFrameWidth = 0;

        /*
         * Indices into tileFrames that have animationSpeed > 0.
         * update() only touches these entries, not the whole map.
         */
        std::vector<int> animatedTileIndices;

        Weather currentWeather = Weather::Clear;
        float animationAccum = 0.0f;
        const float ANIMATION_FRAME_TIME = 1.0f;
        const int TILE_SIZE = 16;
        int spritesheetCols = 0;

        int coord1Based(int row, int col) const {
            return (row - 1) * spritesheetCols + (col - 1);
        };
        int getTerrainSpriteIndex(TerrainType type, int x, int y) const;
        int getTileFrameIndex(const TileFrame& frame) const;
        int getAnimationFrameIndex(TerrainType type, int animFrame) const;
        TerrainLayer getTerrainLayer(TerrainType type) const;

      public:
        MapRenderer(Map* map);
        ~MapRenderer();

        void loadSpritesheets();
        void unloadSpritesheets();
        void initializeTileFrames();

        void update(float deltaTime);
        void render(IO::Graphics::Camera* camera);

        Vector2 getTileAtScreenPos(
            const Vector2& screenPos,
            IO::Graphics::Camera* camera
        ) const;

        void setWeather(Weather weather);
        Weather getWeather() const {
            return currentWeather;
        }
    };
} // namespace OpenWars::Game