#pragma once

#include <cmath>

namespace OpenWars {
    struct Vector2 {
        float x, y;

        Vector2(float x = 0, float y = 0)
            : x(x)
            , y(y) {
        }

        float getLength() const {
            return std::sqrt(x * x + y * y);
        }

        float getDistance(const Vector2& v) const {
            return (*this - v).getLength();
        }

        Vector2 normalize() const {
            return *this / getLength();
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

        float getLength() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        float getDistance(const Vector3& v) const {
            return (*this - v).getLength();
        }

        Vector3 normalize() const {
            float len = getLength();
            if(len == 0.0f)
                return Vector3{0, 0, 0};
            return *this / len;
        }

        float dotProduct(const Vector3& v) const {
            return x * v.x + y * v.y + z * v.z;
        }

        Vector3 crossProduct(const Vector3& v) const {
            return Vector3{
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            };
        }

        Vector3 operator*(float mult) const {
            return Vector3{x * mult, y * mult, z * mult};
        }

        Vector3 operator/(float div) const {
            if(div == 0.0f) {
                return Vector3{0, 0, 0};
            }
            return Vector3{x / div, y / div, z / div};
        }

        Vector3& operator*=(float mult) {
            x *= mult;
            y *= mult;
            z *= mult;
            return *this;
        }

        Vector3& operator/=(float div) {
            if(div == 0.0f) {
                return *this;
            }
            x /= div;
            y /= div;
            z /= div;
            return *this;
        }

        Vector3 operator+(const Vector3& v) const {
            return Vector3{x + v.x, y + v.y, z + v.z};
        }

        Vector3 operator-(const Vector3& v) const {
            return Vector3{x - v.x, y - v.y, z - v.z};
        }

        Vector3& operator+=(const Vector3& v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vector3& operator-=(const Vector3& v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        Vector3 operator-() const {
            return Vector3{-x, -y, -z};
        }
    };
} // namespace OpenWars