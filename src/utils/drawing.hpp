#pragma once

#include <string>
#include "../io/graphics/graphics.hpp"

namespace OpenWars::Utils::Drawing {
    void drawParallelogram(
        IO::Graphics::Vector2 position,
        float width,
        float height,
        float skew,
        IO::Graphics::Color color
    );
    void drawParallelogramOutline(
        IO::Graphics::Vector2 position,
        float width,
        float height,
        float skew,
        IO::Graphics::Color color,
        float thickness
    );
    bool pointInQuad(
        IO::Graphics::Vector2 p,
        IO::Graphics::Vector2 a,
        IO::Graphics::Vector2 b,
        IO::Graphics::Vector2 c,
        IO::Graphics::Vector2 d
    );
    void drawTextWrapped(
        const std::string& text,
        int x,
        int y,
        int maxWidth,
        int fontSize,
        IO::Graphics::Color color
    );
} // namespace OpenWars::Utils::Drawing