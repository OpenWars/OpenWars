#pragma once

#include "../vector.hpp"
#include "shapes.hpp"

namespace OpenWars::Drawing {
    bool checkCollisionPointRec(Vector2 point, Drawing::Rectangle rec);
    bool
    checkCollisionPointTriangle(Vector2 p, Vector2 v1, Vector2 v2, Vector2 v3);
} // namespace OpenWars::Drawing