#pragma once

#include <cstdint>
namespace OpenWars {
    struct Color {
        uint8_t r, g, b, a;

        Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
            : r(r)
            , g(g)
            , b(b)
            , a(a) {
        }
    };
} // namespace OpenWars

namespace OpenWars::Colors {
    inline Color alpha(Color color, float alpha) {
        color.a = (uint8_t)(alpha * 255.0f);
        return color;
    }

    inline Color brightness(Color color, float factor) {
        if(factor > 0) {
            color.r = (uint8_t)(color.r + (255 - color.r) * factor);
            color.g = (uint8_t)(color.g + (255 - color.g) * factor);
            color.b = (uint8_t)(color.b + (255 - color.b) * factor);
        } else {
            float absFactor = 1.0f + factor;
            color.r = (uint8_t)(color.r * absFactor);
            color.g = (uint8_t)(color.g * absFactor);
            color.b = (uint8_t)(color.b * absFactor);
        }
        return color;
    }

    const Color ZINC_50 = {250, 250, 250, 255};
    const Color ZINC_100 = {244, 244, 245, 255};
    const Color ZINC_200 = {228, 228, 231, 255};
    const Color ZINC_300 = {212, 212, 216, 255};
    const Color ZINC_400 = {161, 161, 170, 255};
    const Color ZINC_500 = {113, 113, 122, 255};
    const Color ZINC_600 = {82, 82, 91, 255};
    const Color ZINC_700 = {63, 63, 70, 255};
    const Color ZINC_800 = {39, 39, 42, 255};
    const Color ZINC_900 = {24, 24, 27, 255};
    const Color ZINC_950 = {9, 9, 11, 255};

    const Color GREEN_50 = {240, 253, 244, 255};
    const Color GREEN_100 = {220, 252, 231, 255};
    const Color GREEN_200 = {187, 247, 208, 255};
    const Color GREEN_300 = {134, 239, 172, 255};
    const Color GREEN_400 = {74, 222, 128, 255};
    const Color GREEN_500 = {34, 197, 94, 255};
    const Color GREEN_600 = {22, 163, 74, 255};
    const Color GREEN_700 = {21, 128, 61, 255};
    const Color GREEN_800 = {22, 101, 52, 255};
    const Color GREEN_900 = {20, 83, 45, 255};
    const Color GREEN_950 = {5, 46, 22, 255};

    const Color BLACK = {0, 0, 0, 255};
    const Color WHITE = {255, 255, 255, 255};
} // namespace OpenWars::Colors