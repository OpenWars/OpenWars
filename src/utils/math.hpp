#pragma once

namespace OpenWars::Utils::Math {
    float lerp(float a, float b, float t);
    float easeOutQuad(float t);
    float easeOutCubic(float t);
    float easeInOutCubic(float t);
    float smoothstep(float t);
} // namespace OpenWars::Utils::Math