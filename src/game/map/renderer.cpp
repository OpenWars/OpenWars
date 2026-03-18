#include "renderer.hpp"
#include "../map/map.hpp"
#include "../../io/log/logging.hpp"
#include "terrain.hpp"
#include <asm-generic/errno.h>

using namespace OpenWars::IO::Graphics;
using namespace OpenWars::Drawing;

OpenWars::Game::MapRenderer::MapRenderer(Map* map)
    : gameMap(map) {
}

OpenWars::Game::MapRenderer::~MapRenderer() {
    unloadSpritesheets();
}

void OpenWars::Game::MapRenderer::loadSpritesheets() {
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

void OpenWars::Game::MapRenderer::unloadSpritesheets() {
    for(auto& [weather, sheet] : spritesheets)
        delete sheet;
    spritesheets.clear();
}

/*
 * Connection-mask bit layout:  N=bit0  S=bit1  E=bit2  W=bit3
 *
 * Rotation transforms connections 90° CW per step:
 *   N→E  E→S  S→W  W→N
 *
 * All 16 connection masks can be expressed with these four sprites + rotation,
 * except the five degenerate cases (0 or 1 connections), which have no
 * dedicated sprite and are approximated with the nearest straight tile.
 */
OpenWars::Game::MapRenderer::SpriteSelection
OpenWars::Game::MapRenderer::getRoadSpriteAndRotation(int mask) {
    switch(mask) {
    case 0b0000:
        return {2, 9, 0}; // isolated   → EW straight
    case 0b0001:
        return {2, 9, 1}; // N only     → NS straight
    case 0b0010:
        return {2, 9, 1}; // S only     → NS straight
    case 0b0100:
        return {2, 9, 0}; // E only     → EW straight
    case 0b1000:
        return {2, 9, 0}; // W only     → EW straight
        //
    case 0b0011:
        return {2, 9, 1}; // NS  (EW rotated 90°CW)
    case 0b1100:
        return {2, 9, 0}; // EW  (base)
    case 0b0110:
        return {2, 8, 0}; // SE  (base)
    case 0b1010:
        return {2, 8, 1}; // SW  (SE rot 90°CW)
    case 0b1001:
        return {2, 8, 2}; // NW  (SE rot 180°)
    case 0b0101:
        return {2, 8, 3}; // NE  (SE rot 270°CW)
    case 0b1110:
        return {2, 10, 0}; // SEW / EWS  (base, missing N)
    case 0b1011:
        return {2, 10, 1}; // NSW        (EWS rot 90°CW,  missing E)
    case 0b1101:
        return {2, 10, 2}; // NEW        (EWS rot 180°,   missing S)
    case 0b0111:
        return {2, 10, 3}; // NSE        (EWS rot 270°CW, missing W)
    case 0b1111:
        return {3, 10, 0}; // NSEW
    default:
        return {2, 9, 0}; // unreachable
    }
}

/*
 * This function always returns row 5 for the straight case (col 3).
 * initializeTileFrames bumps row to 6 when y is odd.
 *
 * T-junctions and cross have no sprite; approximated with straight.
 */
OpenWars::Game::MapRenderer::SpriteSelection
OpenWars::Game::MapRenderer::getRiverSpriteAndRotation(int mask) {
    switch(mask) {
    // Straight
    case 0b0011:
        return {5, 3, 0}; // NS  (base)
    case 0b1100:
        return {4, 3, 0}; // EW  (NS rot 90°CW)
    // Corners
    case 0b0110:
        return {5, 4, 0}; // SE  (base)
    case 0b1010:
        return {5, 4, 1}; // SW  (SE rot 90°CW)
    case 0b1001:
        return {5, 4, 2}; // NW  (SE rot 180°)
    case 0b0101:
        return {5, 4, 3}; // NE  (SE rot 270°CW)
    // T-junctions
    case 0b0111:
        return {6, 4, 1}; // NSE → NS straight
    case 0b1011:
        return {6, 4, 2}; // NSW → NS straight
    case 0b1110:
        return {6, 4, 3}; // SEW → EW straight
    case 0b1101:
        return {6, 4, 0}; // NEW → EW straight
    // Cross
    case 0b1111:
        return {6, 3, 0};
    // Isolated
    default: {
        bool hasNS = mask & 0b0011;
        return {4, 3, hasNS ? 0 : 1};
    }
    }
}

/*
 * Mask bits indicate which neighbours are TerrainType::Sea.
 * T-junctions and opposite-sides have no sprite; approximated.
 */
OpenWars::Game::MapRenderer::SpriteSelection
OpenWars::Game::MapRenderer::getCoastSpriteAndRotation(int mask) {
    switch(mask) {
    case 0b0100:
        return {6, 1, 0}; // sea E            (base)
    case 0b0010:
        return {6, 1, 1}; // sea S            (rot 90°CW)
    case 0b1000:
        return {6, 1, 2}; // sea W            (rot 180°)
    case 0b0001:
        return {6, 1, 3}; // sea N            (rot 270°CW)
    case 0b0110:
        return {5, 1, 0}; // sea SE           (base)
    case 0b1010:
        return {5, 1, 1}; // sea SW           (rot 90°CW)
    case 0b1001:
        return {5, 1, 2}; // sea NW           (rot 180°)
    case 0b0101:
        return {5, 1, 3}; // sea NE           (rot 270°CW)
    case 0b0011:
        return {6, 1, 3}; // sea N+S → N straight
    case 0b1100:
        return {6, 1, 0}; // sea E+W → E straight
    case 0b0111:
        return {6, 1, 0}; // sea N+S+E → E straight
    case 0b1011:
        return {6, 1, 2}; // sea N+S+W → W straight
    case 0b1110:
        return {6, 1, 1}; // sea S+E+W → S straight
    case 0b1101:
        return {6, 1, 3}; // sea N+E+W → N straight
    default:
        return {6, 1, 0};
    }
}

void OpenWars::Game::MapRenderer::initializeTileFrames() {
    if(!gameMap)
        return;

    const int width = gameMap->getWidth();
    const int height = gameMap->getHeight();
    tileFrameWidth = width;

    tileFrames.resize(width * height);
    animatedTileIndices.clear();

    auto isRoadLike = [](TerrainType t) {
        switch(t) {
        case TerrainType::Road:
        case TerrainType::Bridge:
        case TerrainType::City:
        case TerrainType::Factory:
        case TerrainType::HQ:
        case TerrainType::Airport:
        case TerrainType::Port:
        case TerrainType::CommTower:
        case TerrainType::Lab:
            return true;
        default:
            return false;
        }
    };

    auto isRiverLike = [](TerrainType t) {
        return t == TerrainType::River || t == TerrainType::Sea ||
               t == TerrainType::Coast;
    };

    // For coast: only pure Sea neighbours determine which edge faces the sea.
    auto isLandLike = [](TerrainType t) {
        return t != TerrainType::Sea && t != TerrainType::River &&
               t != TerrainType::Reef;
    };

    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            const int flat = y * width + x;
            TileFrame& frame = tileFrames[flat];
            const TerrainType type = gameMap->getTerrain(x, y)->getType();

            frame.base.spriteIndex = getTerrainSpriteIndex(type, x, y);

            if(type == TerrainType::HighMountain) {
                // todo: unhardcode this
                frame.overlayIndex = coord1Based(1, 6);
            } else if(type == TerrainType::Road) {
                int mask = computeConnectionMask(x, y, isRoadLike);
                auto sel = getRoadSpriteAndRotation(mask);
                frame.base.spriteIndex = coord1Based(sel.row, sel.col);
                frame.rotation = sel.rotation;
            } else if(type == TerrainType::River) {
                int mask = computeConnectionMask(x, y, isRiverLike);
                auto sel = getRiverSpriteAndRotation(mask);
                frame.base.spriteIndex = coord1Based(sel.row, sel.col);
                frame.rotation = sel.rotation;
            } else if(type == TerrainType::Coast) {
                constexpr int cdx[] = {0, 0, 1, -1};
                constexpr int cdy[] = {-1, 1, 0, 0};
                int landMask = 0;
                for(int d = 0; d < 4; ++d) {
                    auto* t = gameMap->getTerrain(x + cdx[d], y + cdy[d]);
                    if(!t || isLandLike(t->getType()))
                        landMask |= (1 << d);
                }
                int seaMask = (~landMask) & 0b1111;

                // No cardinal sea: check diagonals for inner-corner
                // detection.
                if(seaMask == 0) {
                    // Order: NE(0) SE(1) SW(2) NW(3)
                    constexpr int ddx[] = {1, 1, -1, -1};
                    constexpr int ddy[] = {-1, 1, 1, -1};
                    // Each diagonal sea maps to the matching cardinal pair
                    // mask
                    constexpr int diagToSeaMask[] = {
                        0b0101, // NE diag → sea N+E
                        0b0110, // SE diag → sea S+E
                        0b1010, // SW diag → sea S+W
                        0b1001, // NW diag → sea N+W
                    };
                    for(int d = 0; d < 4; ++d) {
                        auto* t = gameMap->getTerrain(x + ddx[d], y + ddy[d]);
                        if(t && !isLandLike(t->getType())) {
                            seaMask = diagToSeaMask[d];
                            break;
                        }
                    }
                }

                auto sel = getCoastSpriteAndRotation(seaMask);
                frame.base.spriteIndex = coord1Based(sel.row, sel.col);
                frame.rotation = sel.rotation;

                frame.underlay.spriteIndex = coord1Based(3, 1);
                frame.underlay.animationSpeed = 1.0f;
                frame.underlay.frameCount = 3;
            } else if(type == TerrainType::Reef) {
                frame.underlay.spriteIndex = coord1Based(3, 1);
                frame.underlay.animationSpeed = 1.0f;
                frame.underlay.frameCount = 3;
            } else if(type == TerrainType::Sea) {
                // If all 4 cardinal points are plains, then display as a hole
                // at 8,1
                bool allPlain = true;
                for(int dy = -1; dy <= 1 && allPlain; ++dy)
                    for(int dx = -1; dx <= 1 && allPlain; ++dx) {
                        if(dx == 0 && dy == 0)
                            continue;
                        auto* t = gameMap->getTerrain(x + dx, y + dy);
                        if(!t || t->getType() != TerrainType::Plain)
                            allPlain = false;
                    }

                if(allPlain) {
                    frame.base.spriteIndex = coord1Based(8, 1);
                    frame.underlay.spriteIndex = coord1Based(3, 1);
                    frame.underlay.animationSpeed = 1.0f;
                    frame.underlay.frameCount = 3;
                } else {
                    frame.base.animationSpeed = 1.0f;
                    frame.base.frameCount = 3;
                    frame.base.spriteIndex = coord1Based(3, 1);
                }
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

int OpenWars::Game::MapRenderer::getTerrainSpriteIndex(
    TerrainType type,
    int x,
    int y
) const {
    switch(type) {
    case TerrainType::Plain:
        return coord1Based(1, 1);
    case TerrainType::Woods:
        return (y % 2 == 0) ? coord1Based(1, 4) : coord1Based(1, 3);
    case TerrainType::Mountain:
        return coord1Based(2, 5);
    case TerrainType::HighMountain:
        return coord1Based(2, 6);
    case TerrainType::Sea:
        return coord1Based(3, 1);
    case TerrainType::Reef:
        return coord1Based(3, 4);
    case TerrainType::Coast:
        return coord1Based(5, 1);
    case TerrainType::River:
        return coord1Based(5, 3);
    case TerrainType::Road:
        return coord1Based(2, 9);
    case TerrainType::Bridge:
        return coord1Based(4, 8);
    default:
        return coord1Based(1, 1);
    }
}

int OpenWars::Game::MapRenderer::getTileFrameIndex(
    const AnimState& anim
) const {
    if(anim.frameCount <= 1)
        return anim.spriteIndex;
    int f = static_cast<int>(anim.animationTime / ANIMATION_FRAME_TIME) %
            anim.frameCount;
    return anim.spriteIndex + f * anim.frameStride;
}

OpenWars::Game::MapRenderer::TerrainLayer
OpenWars::Game::MapRenderer::getTerrainLayer(TerrainType type) const {
    switch(type) {
    case TerrainType::Woods:
    case TerrainType::Mountain:
    case TerrainType::HighMountain:
    case TerrainType::HQ:
    case TerrainType::Factory:
    case TerrainType::Airport:
    case TerrainType::Port:
    case TerrainType::Lab:
    case TerrainType::CommTower:
    case TerrainType::Silo:
    case TerrainType::Road:
    case TerrainType::Coast:
    case TerrainType::Reef:
        return TerrainLayer::Foreground;
    default:
        return TerrainLayer::Background;
    }
}

void OpenWars::Game::MapRenderer::update(float deltaTime) {
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

OpenWars::Vector2 OpenWars::Game::MapRenderer::getTileAtScreenPos(
    const Vector2& screenPos,
    IO::Graphics::Camera* camera
) const {
    if(!camera || !gameMap)
        return Vector2{-1, -1};

    Vector3 cameraPos = camera->getPosition();
    float zoom = camera->getZoom();
    int viewportW = camera->getViewportWidth();
    int viewportH = camera->getViewportHeight();

    float scaledTileSize = TILE_SIZE * zoom;
    float camOffsetX = viewportW / 2.0f - (cameraPos.x * zoom);
    float camOffsetY = viewportH / 2.0f - (cameraPos.y * zoom);

    int tileX = (int)((screenPos.x - camOffsetX) / scaledTileSize);
    int tileY = (int)((screenPos.y - camOffsetY) / scaledTileSize);

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

void OpenWars::Game::MapRenderer::render(IO::Graphics::Camera* camera) {
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

    Vector3 cameraPos = camera->getPosition();
    float zoom = camera->getZoom();
    int viewportW = camera->getViewportWidth();
    int viewportH = camera->getViewportHeight();

    float scaledTileSize = TILE_SIZE * zoom;
    float camOffsetX = viewportW / 2.0f - (cameraPos.x * zoom);
    float camOffsetY = viewportH / 2.0f - (cameraPos.y * zoom);

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

    for(int y = minY; y <= maxY; ++y) {
        for(int x = minX; x <= maxX; ++x) {
            Terrain* terrain = gameMap->getTerrain(x, y);
            if(!terrain)
                continue;
            if(getTerrainLayer(terrain->getType()) != TerrainLayer::Foreground)
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

    // Pass 2: all terrain with per-tile rotation.
    for(int y = minY; y <= maxY; ++y) {
        for(int x = minX; x <= maxX; ++x) {
            float screenX = (x * scaledTileSize) + camOffsetX;
            float screenY = (y * scaledTileSize) + camOffsetY;

            const int flat = y * tileFrameWidth + x;
            const TileFrame& frame = tileFrames[flat];
            int frameIndex = getTileFrameIndex(frame.base);
            sheet
                ->drawFrame(frameIndex, screenX, screenY, zoom, frame.rotation);
        }
    }

    // Pass 3: upper-tile overlays (e.g. HighMountain top half).
    // Iterate y from minY-1 so tiles at minY can still paint their overlay
    // above the visible area (the draw call clips naturally).
    for(int y = minY; y <= maxY; ++y) {
        for(int x = minX; x <= maxX; ++x) {
            const int flat = y * tileFrameWidth + x;
            const TileFrame& frame = tileFrames[flat];
            if(frame.overlayIndex == 0)
                continue;

            float screenX = (x * scaledTileSize) + camOffsetX;
            float screenY = ((y - 1) * scaledTileSize) + camOffsetY;
            sheet->drawFrame(frame.overlayIndex, screenX, screenY, zoom);
        }
    }
}

void OpenWars::Game::MapRenderer::setWeather(Weather weather) {
    if(currentWeather != weather) {
        currentWeather = weather;
        IO::Logging::debug("Weather changed to %d", static_cast<int>(weather));
    }
}

template <typename Pred>
int OpenWars::Game::MapRenderer::computeConnectionMask(
    int x,
    int y,
    Pred connectable
) const {
    // N=bit0  S=bit1  E=bit2  W=bit3
    constexpr int dx[] = {0, 0, 1, -1};
    constexpr int dy[] = {-1, 1, 0, 0};
    int mask = 0;
    for(int d = 0; d < 4; ++d) {
        auto* t = gameMap->getTerrain(x + dx[d], y + dy[d]);
        if(t && connectable(t->getType()))
            mask |= (1 << d);
    }
    return mask;
}