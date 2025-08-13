#pragma once

#include "../ui/components/components.hpp"

namespace raylib {
#include "raylib.h"
}

namespace OpenWars {
    namespace Utils {
        namespace Drawing {
            void drawParallelogram(raylib::Vector2 position, float width,
                                   float height, float skew,
                                   raylib::Color color);
            void drawParallelogramOutline(raylib::Vector2 position, float width,
                                          float height, float skew,
                                          raylib::Color color, float thickness);
            bool pointInQuad(raylib::Vector2 p, raylib::Vector2 a,
                             raylib::Vector2 b, raylib::Vector2 c,
                             raylib::Vector2 d);
            void drawTextWrapped(const char *text, int x, int y, int maxWidth,
                                 int fontSize, raylib::Color color);
            void
            renderButtons(raylib::Vector2 parallelogramPos,
                          std::vector<OpenWars::UI::ButtonComponent*> buttons,
                          float width, float buttonAreaHeight);
        } // namespace Drawing
    } // namespace Utils
} // namespace OpenWars