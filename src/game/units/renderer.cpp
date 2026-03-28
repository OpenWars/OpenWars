#include "renderer.hpp"
#include "../map/map.hpp"
#include "../units/unit.hpp"
#include "../../core/drawing/shapes.hpp"
#include "../../core/drawing/text.hpp"
#include "../../io/log/logging.hpp"
#include <algorithm>
#include <string>

using namespace OpenWars::IO::Graphics;
using namespace OpenWars::Drawing;

namespace OpenWars::Game {

    UnitRenderer::UnitRenderer() = default;

    UnitRenderer::~UnitRenderer() {
        for(auto& [weather, sheet] : spritesheets)
            delete sheet;
    }

    void UnitRenderer::loadSpritesheets() {
        auto* sheet = Drawing::SpriteSheet::loadFromAssets(
            "sprites/units_clear.png",
            TILE_SIZE,
            TILE_SIZE
        );

        if(sheet) {
            spritesheetCols = sheet->getCols();
            spritesheets[static_cast<int>(Weather::Clear)] = sheet;
        } else {
            IO::Logging::warn(
                "%s",
                "Unit spritesheet unavailable, using fallback rendering"
            );
        }
    }

    void UnitRenderer::update(float deltaTime) {
        animationAccum += deltaTime;
        if(animationAccum >= ANIMATION_FRAME_TIME) {
            animationAccum -= ANIMATION_FRAME_TIME;
            ++animationFrame;
        }
    }

    void UnitRenderer::render(const Map* map, IO::Graphics::Camera* camera) {
        if(!map || !camera)
            return;

        auto it = spritesheets.find(static_cast<int>(currentWeather));
        Drawing::SpriteSheet* sheet =
            it != spritesheets.end() ? it->second : nullptr;

        Vector3 cameraPos = camera->getPosition();
        float zoom = camera->getZoom();
        int viewportW = camera->getViewportWidth();
        int viewportH = camera->getViewportHeight();

        float scaledTileSize = TILE_SIZE * zoom;
        float camOffsetX = viewportW / 2.0f - (cameraPos.x * zoom);
        float camOffsetY = viewportH / 2.0f - (cameraPos.y * zoom);

        const auto& units = map->getUnits();

        std::vector<const Unit*> sorted;
        sorted.reserve(units.size());
        for(const auto& u : units)
            sorted.push_back(u.get());

        std::sort(
            sorted.begin(),
            sorted.end(),
            [](const Unit* a, const Unit* b) {
                return a->getGridPos().y < b->getGridPos().y;
            }
        );

        for(const Unit* unit : sorted) {
            Vector2 pos = unit->getGridPos();
            float screenX = pos.x * scaledTileSize + camOffsetX;
            float screenY = pos.y * scaledTileSize + camOffsetY;

            if(screenX + scaledTileSize < 0 || screenX > viewportW ||
               screenY + scaledTileSize < 0 || screenY > viewportH)
                continue;

            const UnitDefinition& def = unit->getDef();
            Color teamColor = unit->getTeamColor();

            if(sheet && def.sprite.row != 0) {
                int frameIdx = coord1Based(def.sprite.row, def.sprite.col);
                if(def.sprite.frameCount > 1)
                    frameIdx += animationFrame % def.sprite.frameCount;

                sheet->setColorMod(teamColor.r, teamColor.g, teamColor.b);
                sheet->drawFrame(frameIdx, screenX, screenY, zoom);
                sheet->resetColorMod();
            } else {
                float inset = scaledTileSize * 0.1f;
                float size = scaledTileSize * 0.8f;
                drawRectangle(
                    screenX + inset,
                    screenY + inset,
                    size,
                    size,
                    teamColor
                );
                drawRectangleOutline(
                    screenX + inset,
                    screenY + inset,
                    size,
                    size,
                    Colors::brightness(teamColor, 0.3f)
                );
            }

            if(unit->getHasActed()) {
                drawRectangle(
                    screenX,
                    screenY,
                    scaledTileSize,
                    scaledTileSize,
                    Colors::alpha(Colors::ZINC_900, 0.5f)
                );
            }

            int displayHP = unit->getDisplayHP();
            if(displayHP < 10) {
                int hpSize = (int)std::max(8.0f, std::min(14.0f, zoom * 6.0f));
                std::string hpStr = std::to_string(displayHP);
                float hpX = screenX + scaledTileSize - hpSize;
                float hpY = screenY + scaledTileSize - hpSize;

                Color hpColor = displayHP > 6   ? Colors::WHITE
                                : displayHP > 3 ? Colors::YELLOW_300
                                                : Colors::RED_400;

                Drawing::drawText(
                    hpStr.c_str(),
                    (int)hpX + 1,
                    (int)hpY + 1,
                    hpSize,
                    Colors::BLACK
                );
                Drawing::drawText(
                    hpStr.c_str(),
                    (int)hpX,
                    (int)hpY,
                    hpSize,
                    hpColor
                );
            }
        }
    }

    void UnitRenderer::setWeather(Weather weather) {
        currentWeather = weather;
    }

} // namespace OpenWars::Game