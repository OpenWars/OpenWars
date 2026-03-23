#include "renderer.hpp"
#include "map.hpp"
#include "terrain.hpp"
#include "../../io/log/logging.hpp"
#include <algorithm>
#include <cmath>

using namespace OpenWars::IO::Graphics;
using namespace OpenWars::Drawing;

namespace OpenWars::Game {

    MapRenderer::MapRenderer(Map* map)
        : gameMap(map) {
    }

    MapRenderer::~MapRenderer() {
        unloadSpritesheets();
    }

    void MapRenderer::loadSpritesheets() {
        SpriteSheet* sheet = SpriteSheet::loadFromAssets(
            "sprites/terrain_clear.png",
            TILE_SIZE,
            TILE_SIZE
        );

        if(!sheet) {
            IO::Logging::error(
                false,
                "Failed to load spritesheet: %s",
                "sprites/terrain_clear.png"
            );
            return;
        }

        spritesheetCols = sheet->getCols();
        spritesheets[static_cast<int>(Weather::Clear)] = sheet;
    }

    void MapRenderer::unloadSpritesheets() {
        for(auto& [weather, sheet] : spritesheets)
            delete sheet;
        spritesheets.clear();
    }

    static bool groupConnects(const TileDefinition* def, int neighborGroup) {
        for(int g : def->connectsTo) {
            if(g == neighborGroup)
                return true;
        }
        return false;
    }

    void MapRenderer::initializeTileFrames() {
        if(!gameMap)
            return;

        const int width = gameMap->getWidth();
        const int height = gameMap->getHeight();
        tileFrameWidth = width;

        tileFrames.assign(width * height, TileFrame{});
        animatedTileIndices.clear();

        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                const Terrain* terrain = gameMap->getTerrain(x, y);
                if(!terrain)
                    continue;

                const TileDefinition* def = terrain->getDef();
                if(!def)
                    continue;

                const int flat = y * width + x;
                TileFrame& frame = tileFrames[flat];

                if(def->connectionGroup != 0 && !def->connectsTo.empty()) {
                    int mask =
                        computeConnectionMask(x, y, [&](const Terrain* t) {
                            return t && t->getDef() &&
                                   groupConnects(
                                       def,
                                       t->getDef()->connectionGroup
                                   );
                        });

                    if(def->diagonalConnectionFallback && mask == 0) {
                        constexpr int ddx[] = {1, 1, -1, -1};
                        constexpr int ddy[] = {-1, 1, 1, -1};
                        constexpr int diagToMask[] =
                            {0b0101, 0b0110, 0b1010, 0b1001};
                        for(int d = 0; d < 4; ++d) {
                            const Terrain* t =
                                gameMap->getTerrain(x + ddx[d], y + ddy[d]);
                            if(t && t->getDef() &&
                               groupConnects(
                                   def,
                                   t->getDef()->connectionGroup
                               )) {
                                mask = diagToMask[d];
                                break;
                            }
                        }
                    }

                    const auto& entry = def->connectionSprites.byMask[mask];
                    frame.base.spriteIndex = coord1Based(entry.row, entry.col);
                    frame.rotation = entry.rotation;

                } else {
                    int row = def->sprite.row;
                    int col = def->sprite.col;
                    if(def->sprite.altRow != 0 && y % 2 != 0) {
                        row = def->sprite.altRow;
                        col = def->sprite.altCol;
                    }
                    frame.base.spriteIndex = coord1Based(row, col);
                    frame.base.animationSpeed = def->sprite.animSpeed;
                    frame.base.frameCount = def->sprite.frameCount;
                }

                if(def->overlay.row != 0)
                    frame.overlayIndex =
                        coord1Based(def->overlay.row, def->overlay.col);

                if(def->underlay.row != 0) {
                    frame.underlay.spriteIndex =
                        coord1Based(def->underlay.row, def->underlay.col);
                    frame.underlay.animationSpeed = def->underlay.animSpeed;
                    frame.underlay.frameCount = def->underlay.frameCount;
                }

                if(frame.base.animationSpeed > 0.0f ||
                   frame.underlay.animationSpeed > 0.0f)
                    animatedTileIndices.push_back(flat);
            }
        }

        IO::Logging::debug(
            "Animated tile count: %d",
            (int)animatedTileIndices.size()
        );
    }

    int MapRenderer::getTileFrameIndex(const AnimState& anim) const {
        if(anim.frameCount <= 1)
            return anim.spriteIndex;
        int f = static_cast<int>(anim.animationTime / ANIMATION_FRAME_TIME) %
                anim.frameCount;
        return anim.spriteIndex + f * anim.frameStride;
    }

    void MapRenderer::update(float deltaTime) {
        if(animatedTileIndices.empty())
            return;

        animationAccum += deltaTime;
        if(animationAccum < ANIMATION_FRAME_TIME)
            return;
        animationAccum -= ANIMATION_FRAME_TIME;

        for(int flat : animatedTileIndices) {
            tileFrames[flat].base.animationTime +=
                tileFrames[flat].base.animationSpeed;
            tileFrames[flat].underlay.animationTime +=
                tileFrames[flat].underlay.animationSpeed;
        }
    }

    Vector2 MapRenderer::getTileAtScreenPos(
        const Vector2& screenPos,
        IO::Graphics::Camera* camera
    ) const {
        if(!camera || !gameMap)
            return Vector2{-1, -1};

        Vector3 world = camera->screenToWorld(screenPos, 0.0f);
        int tileX = (int)(world.x / TILE_SIZE);
        int tileY = (int)(world.y / TILE_SIZE);

        if(gameMap->isInBounds(tileX, tileY))
            return Vector2{(float)tileX, (float)tileY};
        return Vector2{-1, -1};
    }

    static void computeVisibleRange(
        float camOffsetX,
        float camOffsetY,
        float scaledTileSize,
        int viewportW,
        int viewportH,
        int mapWidth,
        int mapHeight,
        int& outMinX,
        int& outMaxX,
        int& outMinY,
        int& outMaxY
    ) {
        outMinX = std::max(0, (int)std::floor(-camOffsetX / scaledTileSize));
        outMaxX = std::min(
            mapWidth - 1,
            (int)std::floor((viewportW - camOffsetX) / scaledTileSize)
        );
        outMinY = std::max(0, (int)std::floor(-camOffsetY / scaledTileSize));
        outMaxY = std::min(
            mapHeight - 1,
            (int)std::floor((viewportH - camOffsetY) / scaledTileSize)
        );
    }

    void MapRenderer::render(IO::Graphics::Camera* camera) {
        if(tileFrames.empty() || !camera || !gameMap)
            return;

        auto it = spritesheets.find(static_cast<int>(currentWeather));
        if(it == spritesheets.end())
            return;

        SpriteSheet* sheet = it->second;
        if(!sheet)
            return;

        const int mapWidth = gameMap->getWidth();
        const int mapHeight = gameMap->getHeight();

        float zoom = camera->getZoom();
        int viewportW = camera->getViewportWidth();
        int viewportH = camera->getViewportHeight();

        float scaledTileSize = TILE_SIZE * zoom;

        // Derive screen-space origin from the camera matrices so the renderer
        // stays connected to the full matrix pipeline rather than replicating
        // the camera math here.
        Vector2 originScreen = camera->worldToScreen(Vector3{0.0f, 0.0f, 0.0f});
        float camOffsetX = originScreen.x;
        float camOffsetY = originScreen.y;

        int minX, maxX, minY, maxY;
        computeVisibleRange(
            camOffsetX,
            camOffsetY,
            scaledTileSize,
            viewportW,
            viewportH,
            mapWidth,
            mapHeight,
            minX,
            maxX,
            minY,
            maxY
        );

        if(minX > maxX || minY > maxY)
            return;

        const int plainIdx = coord1Based(1, 1);

        // Pass 1: underlay for foreground tiles.
        for(int y = minY; y <= maxY; ++y) {
            for(int x = minX; x <= maxX; ++x) {
                const Terrain* terrain = gameMap->getTerrain(x, y);
                if(!terrain || !terrain->getDef())
                    continue;
                if(!terrain->getDef()->foreground)
                    continue;

                float screenX = (x * scaledTileSize) + camOffsetX;
                float screenY = (y * scaledTileSize) + camOffsetY;

                const int flat = y * tileFrameWidth + x;
                const AnimState& ul = tileFrames[flat].underlay;
                const int under =
                    (ul.spriteIndex != 0) ? getTileFrameIndex(ul) : plainIdx;
                sheet->drawFrame(under, screenX, screenY, zoom);
            }
        }

        // Pass 2: all tiles with per-tile rotation.
        for(int y = minY; y <= maxY; ++y) {
            for(int x = minX; x <= maxX; ++x) {
                float screenX = (x * scaledTileSize) + camOffsetX;
                float screenY = (y * scaledTileSize) + camOffsetY;

                const int flat = y * tileFrameWidth + x;
                const TileFrame& frame = tileFrames[flat];
                sheet->drawFrame(
                    getTileFrameIndex(frame.base),
                    screenX,
                    screenY,
                    zoom,
                    frame.rotation
                );
            }
        }

        // Pass 3: upper-tile overlays (e.g. HighMountain top cap).
        for(int y = minY; y <= maxY; ++y) {
            for(int x = minX; x <= maxX; ++x) {
                const int flat = y * tileFrameWidth + x;
                if(tileFrames[flat].overlayIndex == 0)
                    continue;

                float screenX = (x * scaledTileSize) + camOffsetX;
                float screenY = ((y - 1) * scaledTileSize) + camOffsetY;
                sheet->drawFrame(
                    tileFrames[flat].overlayIndex,
                    screenX,
                    screenY,
                    zoom
                );
            }
        }
    }

    void MapRenderer::setWeather(Weather weather) {
        if(currentWeather != weather) {
            currentWeather = weather;
            IO::Logging::debug(
                "Weather changed to %d",
                static_cast<int>(weather)
            );
        }
    }

    template <typename Pred>
    int MapRenderer::computeConnectionMask(
        int x,
        int y,
        Pred connectable
    ) const {
        // N=bit0  S=bit1  E=bit2  W=bit3
        constexpr int dx[] = {0, 0, 1, -1};
        constexpr int dy[] = {-1, 1, 0, 0};
        int mask = 0;
        for(int d = 0; d < 4; ++d) {
            const Terrain* t = gameMap->getTerrain(x + dx[d], y + dy[d]);
            if(connectable(t))
                mask |= (1 << d);
        }
        return mask;
    }

} // namespace OpenWars::Game