#pragma once

namespace OpenWars {
    struct Vector2 {
        float x, y;

        Vector2(float x = 0, float y = 0)
            : x(x)
            , y(y) {
        }
    };

    struct Vector3 {
        float x, y, z;

        Vector3(float x = 0, float y = 0, float z = 0)
            : x(x)
            , y(y)
            , z(z) {
        }
    };
} // namespace OpenWars