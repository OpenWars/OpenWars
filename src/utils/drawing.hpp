#pragma once

namespace raylib {
    #include "raylib.h"
}

namespace OpenWars {
    namespace Utils {
        namespace Drawing {
            void drawParallelogram(raylib::Vector2 position, float width, float height, float skew, raylib::Color color);
            bool pointInQuad(raylib::Vector2 p, raylib::Vector2 a, raylib::Vector2 b, raylib::Vector2 c, raylib::Vector2 d);
        }
    }
}