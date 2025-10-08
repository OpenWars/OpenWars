#pragma once

#include <string>
#include "../core/colors.hpp"
#include "../core/vector.hpp"

namespace OpenWars::Utils::Drawing {
    void drawParallelogram(
        Vector2 position,
        float width,
        float height,
        float skew,
        Color color
    );
    void drawParallelogramOutline(
        Vector2 position,
        float width,
        float height,
        float skew,
        Color color,
        float thickness
    );
    bool pointInQuad(Vector2 p, Vector2 a, Vector2 b, Vector2 c, Vector2 d);
    void drawTextWrapped(
        const std::string& text,
        int x,
        int y,
        int maxWidth,
        int fontSize,
        Color color
    );
} // namespace OpenWars::Utils::Drawing