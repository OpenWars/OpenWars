#pragma once

namespace raylib {
    #include "raylib.h"
}

namespace OpenWars {
    namespace Utils {
        namespace Drawing {
            void drawQuad(raylib::Vector2 a, raylib::Vector2 b, raylib::Vector2 c, raylib::Vector2 d, raylib::Color color);
            bool pointInQuad(raylib::Vector2 p, raylib::Vector2 a, raylib::Vector2 b, raylib::Vector2 c, raylib::Vector2 d);
        }
    }
}