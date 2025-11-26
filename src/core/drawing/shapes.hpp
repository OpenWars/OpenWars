#pragma once

#include "../colors.hpp"
#include "../vector.hpp"
#include <SDL3/SDL.h>

namespace OpenWars::Drawing {
    struct Rectangle {
        float x, y, width, height;

        Rectangle(float x = 0, float y = 0, float w = 0, float h = 0)
            : x(x)
            , y(y)
            , width(w)
            , height(h) {
        }
    };

    void drawRectangle(int x, int y, int width, int height, Color color);
    void drawRectangleRec(Rectangle rec, Color color);
    void drawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
    void drawLineEx(Vector2 start, Vector2 end, float thickness, Color color);
} // namespace OpenWars::Drawing