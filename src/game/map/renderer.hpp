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
            int overlayIndex = 0;
            float animationTime = 0.0f;
            float animationSpeed = 0.0f;
            int frameCount = 1;
            // Step between animation frames in the flat sprite index.
            // Use 1 for frames laid out in the same row (e.g. Sea: cols 1→2→3).
            // Use spritesheetCols for frames in the same column (e.g. River:
            // rows 5→6 at col 3).
            int frameStride = 1;
            // 0=0° · 1=90°CW · 2=180° · 3=270°CW
            int rotation = 0;
        };

        // Sprite-sheet address (1-based row/col) plus the CW rotation needed
        // to draw a particular tile variant.
        struct SpriteSelection {
            int row, col, rotation;
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

        // Returns 4-bit neighbour-connection mask: N=bit0 S=bit1 E=bit2 W=bit3
        // A neighbour contributes if connectable(its TerrainType) returns true.
        template <typename Pred>
        int computeConnectionMask(int x, int y, Pred connectable) const;

        // Chooses the road sprite + rotation for a given connection mask.
        static SpriteSelection getRoadSpriteAndRotation(int mask);

        // Chooses the river sprite + rotation for a given connection mask.
        static SpriteSelection getRiverSpriteAndRotation(int mask);

        // Chooses the coast sprite + rotation for a given sea-neighbour mask.
        // mask bits indicate which neighbours are TerrainType::Sea.
        static SpriteSelection getCoastSpriteAndRotation(int mask);

        int getTerrainSpriteIndex(TerrainType type, int x, int y) const;
        int getTileFrameIndex(const TileFrame& frame) const;
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