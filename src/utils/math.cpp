#include "math.hpp"

namespace OpenWars::Utils::Math {
    float lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }

    float easeOutQuad(float t) {
        return t * (2.0f - t);
    }

    float easeOutCubic(float t) {
        float f = 1.0f - t;
        return 1.0f - f * f * f;
    }

    float easeInOutCubic(float t) {
        if(t < 0.5f)
            return 4.0f * t * t * t;
        float f = 2.0f * t - 2.0f;
        return 0.5f * f * f * f + 1.0f;
    }

    float smoothstep(float t) {
        return t * t * (3.0f - 2.0f * t);
    }
} // namespace OpenWars::Utils::Math