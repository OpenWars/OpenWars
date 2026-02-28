#pragma once

#include "../../core/drawing/spritesheet.hpp"
#include "../../io/graphics/camera.hpp"
#include <unordered_map>
#include <vector>

namespace OpenWars::Game {
    // Forward declarations
    class Map;
    enum class TerrainType;
    enum class Weather { Clear, Rain, Snow };

    /**
     * Responsible for rendering map tiles and managing terrain animation.
     * Handles spritesheet loading, tile frame management, and rendering logic.
     */
    class MapRenderer {
      private:
        struct TileFrame {
            int spriteIndex;
            float animationTime = 0.0f;
            float animationSpeed = 0.0f;
            int frameCount = 1;
        };

        enum class TerrainLayer {
            Background, // Plains and base terrain
            Foreground  // Woods, mountains, and elevated terrain
        };

        Map* gameMap;
        std::unordered_map<int, Drawing::SpriteSheet*> spritesheets;
        std::vector<std::vector<TileFrame>> tileFrames;

        Weather currentWeather = Weather::Clear;
        float animationAccum = 0.0f;
        const float ANIMATION_FRAME_TIME = 1.0f;
        const int TILE_SIZE = 16;
        int spritesheetCols = 0;

        // Private methods for sprite index calculation
        int getTerrainSpriteIndex(TerrainType type, int x, int y) const;
        int getTileFrameIndex(const TileFrame& frame) const;
        int getAnimationFrameIndex(TerrainType type, int animFrame) const;
        TerrainLayer getTerrainLayer(TerrainType type) const;

      public:
        MapRenderer(Map* map);
        ~MapRenderer();

        // Lifecycle management
        void loadSpritesheets();
        void unloadSpritesheets();
        void initializeTileFrames();

        // Updates
        void update(float deltaTime);

        // Rendering
        void render(IO::Graphics::Camera* camera);

        // Weather management
        void setWeather(Weather weather);
        Weather getWeather() const {
            return currentWeather;
        }
    };
} // namespace OpenWars::Game
