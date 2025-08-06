#include "drawing.hpp"

void OpenWars::Utils::Drawing::drawParallelogram(raylib::Vector2 position, float width, float height, float skew, raylib::Color color) {
    raylib::Vector2 v1 = position;
    raylib::Vector2 v2 = {position.x + width, position.y};
    raylib::Vector2 v3 = {position.x + width + skew, position.y - height};
    raylib::Vector2 v4 = {position.x + skew, position.y - height};
    
    DrawTriangle(v1, v2, v3, color);
    DrawTriangle(v1, v3, v4, color);
}

bool OpenWars::Utils::Drawing::pointInQuad(raylib::Vector2 p, raylib::Vector2 a, raylib::Vector2 b, raylib::Vector2 c, raylib::Vector2 d) {
    return CheckCollisionPointTriangle(p, a, b, c) || CheckCollisionPointTriangle(p, c, d, a);
}