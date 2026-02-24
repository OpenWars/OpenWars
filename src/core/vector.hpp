#pragma once

#include <cmath>

namespace OpenWars {
    struct Vector2 {
        float x, y;

        Vector2(float x = 0, float y = 0)
            : x(x)
            , y(y) {
        }

        float GetLength() const {
            return std::sqrt(x * x + y * y);
        }

        float GetDistance(const Vector2& v) const {
            return (*this - v).GetLength();
        }

        Vector2 Normalize() const {
            return *this / GetLength();
        }

        Vector2 operator*(float mult) const {
            return Vector2{x * mult, y * mult};
        }

        Vector2 operator/(float div) const {
            if(div == 0.0f) {
                return Vector2{0, 0};
            }

            return Vector2{x / div, y / div};
        }

        Vector2& operator*=(float mult) {
            x *= mult;
            y *= mult;
            return *this;
        }

        Vector2& operator/=(float div) {
            if(div == 0.0f) {
                return *this;
            }

            x /= div;
            y /= div;
            return *this;
        }

        Vector2 operator+(const Vector2& v) const {
            return Vector2{x + v.x, y + v.y};
        }

        Vector2 operator-(const Vector2& v) const {
            return *this + (-v);
        }

        Vector2& operator+=(const Vector2& v) {
            x += v.x;
            y += v.y;
            return *this;
        }

        Vector2& operator-=(const Vector2& v) {
            return *this += (-v);
        }

        Vector2 operator-() const {
            return Vector2{-x, -y};
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