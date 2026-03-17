#include "renderer.hpp"
#include "../map/map.hpp"
#include "../../io/log/logging.hpp"

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
        return {5, 3, 1}; // EW  (NS rot 90°CW)
    // Corners
    case 0b0110:
        return {5, 3, 0}; // SE  (base)
    case 0b1010:
        return {5, 3, 1}; // SW  (SE rot 90°CW)
    case 0b1001:
        return {5, 3, 2}; // NW  (SE rot 180°)
    case 0b0101:
        return {5, 3, 3}; // NE  (SE rot 270°CW)
    // T-junctions (todo: sprite)
    case 0b0111:
        return {5, 3, 0}; // NSE → NS straight
    case 0b1011:
        return {5, 3, 0}; // NSW → NS straight
    case 0b1110:
        return {5, 3, 1}; // SEW → EW straight
    case 0b1101:
        return {5, 3, 1}; // NEW → EW straight
    // Cross (todo: sprite)
    case 0b1111:
        return {5, 3, 0};
    // Isolated
    default: {
        bool hasNS = mask & 0b0011;
        return {5, 3, hasNS ? 0 : 1};
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
        return t != TerrainType::Sea && t != TerrainType::River;
    };

    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            const int flat = y * width + x;
            TileFrame& frame = tileFrames[flat];
            const TerrainType type = gameMap->getTerrain(x, y)->getType();

            frame.spriteIndex = getTerrainSpriteIndex(type, x, y);

            if(type == TerrainType::Road) {
                int mask = computeConnectionMask(x, y, isRoadLike);
                auto sel = getRoadSpriteAndRotation(mask);
                frame.spriteIndex = coord1Based(sel.row, sel.col);
                frame.rotation = sel.rotation;
            } else if(type == TerrainType::River) {
                int mask = computeConnectionMask(x, y, isRiverLike);
                auto sel = getRiverSpriteAndRotation(mask);
                frame.spriteIndex = coord1Based(sel.row, sel.col);
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

                // No cardinal sea: check diagonals for inner-corner detection.
                if(seaMask == 0) {
                    // Order: NE(0) SE(1) SW(2) NW(3)
                    constexpr int ddx[] = {1, 1, -1, -1};
                    constexpr int ddy[] = {-1, 1, 1, -1};
                    // Each diagonal sea maps to the matching cardinal pair mask
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
                frame.spriteIndex = coord1Based(sel.row, sel.col);
                frame.rotation = sel.rotation;
            } else if(type == TerrainType::Sea) {
                frame.animationSpeed = 1.0f;
                frame.frameCount = 3;
                frame.spriteIndex = coord1Based(3, 1);
            }

            if(frame.animationSpeed > 0.0f)
                animatedTileIndices.push_back(flat);
        }
    }
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
    case TerrainType::Sea:
        return coord1Based(3, 1);
    case TerrainType::Coast:
        return coord1Based(5, 1);
    case TerrainType::River:
        return coord1Based(5, 3);
    case TerrainType::Road:
        // initializeTileFrames will refine this to the correct variant.
        return coord1Based(2, 9);
    default:
        return coord1Based(1, 1);
    }
}

int OpenWars::Game::MapRenderer::getTileFrameIndex(
    const TileFrame& frame
) const {
    if(frame.frameCount <= 1)
        return frame.spriteIndex;
    int animFrame =
        static_cast<int>(frame.animationTime / ANIMATION_FRAME_TIME) %
        frame.frameCount;
    return frame.spriteIndex + animFrame * frame.frameStride;
}

OpenWars::Game::MapRenderer::TerrainLayer
OpenWars::Game::MapRenderer::getTerrainLayer(TerrainType type) const {
    switch(type) {
    case TerrainType::Woods:
    case TerrainType::Mountain:
    case TerrainType::HQ:
    case TerrainType::Factory:
    case TerrainType::Airport:
    case TerrainType::Port:
    case TerrainType::Lab:
    case TerrainType::CommTower:
    case TerrainType::Silo:
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

    for(int flat : animatedTileIndices)
        tileFrames[flat].animationTime += tileFrames[flat].animationSpeed;
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

    // Pass 1: plain underlay for foreground terrain (no rotation needed)
    for(int y = minY; y <= maxY; ++y) {
        for(int x = minX; x <= maxX; ++x) {
            Terrain* terrain = gameMap->getTerrain(x, y);
            if(!terrain)
                continue;
            if(getTerrainLayer(terrain->getType()) != TerrainLayer::Foreground)
                continue;

            float screenX = (x * scaledTileSize) + camOffsetX;
            float screenY = (y * scaledTileSize) + camOffsetY;
            sheet->drawFrame(plainIdx, screenX, screenY, zoom);
        }
    }

    // Pass 2: all terrain with per-tile rotation.
    for(int y = minY; y <= maxY; ++y) {
        for(int x = minX; x <= maxX; ++x) {
            float screenX = (x * scaledTileSize) + camOffsetX;
            float screenY = (y * scaledTileSize) + camOffsetY;

            const int flat = y * tileFrameWidth + x;
            const TileFrame& frame = tileFrames[flat];
            int frameIndex = getTileFrameIndex(frame);
            sheet
                ->drawFrame(frameIndex, screenX, screenY, zoom, frame.rotation);
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