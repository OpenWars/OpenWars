#include "drawing.hpp"

void OpenWars::Utils::Drawing::drawQuad(raylib::Vector2 a, raylib::Vector2 b, raylib::Vector2 c, raylib::Vector2 d, raylib::Color color) {
    raylib::DrawTriangle(a, b, d, color);
    raylib::DrawTriangle(d, b, c, color);
}

bool OpenWars::Utils::Drawing::pointInQuad(raylib::Vector2 p, raylib::Vector2 a, raylib::Vector2 b, raylib::Vector2 c, raylib::Vector2 d) {
    return CheckCollisionPointTriangle(p, a, b, c) || CheckCollisionPointTriangle(p, c, d, a);
}