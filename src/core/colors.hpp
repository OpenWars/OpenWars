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

    // Red
    const Color RED_50 = {254, 242, 242, 255};
    const Color RED_100 = {254, 226, 226, 255};
    const Color RED_200 = {254, 202, 202, 255};
    const Color RED_300 = {252, 165, 165, 255};
    const Color RED_400 = {248, 113, 113, 255};
    const Color RED_500 = {239, 68, 68, 255};
    const Color RED_600 = {220, 38, 38, 255};
    const Color RED_700 = {185, 28, 28, 255};
    const Color RED_800 = {153, 27, 27, 255};
    const Color RED_900 = {127, 29, 29, 255};
    const Color RED_950 = {69, 10, 10, 255};

    // Orange
    const Color ORANGE_50 = {255, 247, 237, 255};
    const Color ORANGE_100 = {255, 237, 213, 255};
    const Color ORANGE_200 = {254, 215, 170, 255};
    const Color ORANGE_300 = {253, 186, 116, 255};
    const Color ORANGE_400 = {251, 146, 60, 255};
    const Color ORANGE_500 = {249, 115, 22, 255};
    const Color ORANGE_600 = {234, 88, 12, 255};
    const Color ORANGE_700 = {194, 65, 12, 255};
    const Color ORANGE_800 = {154, 52, 18, 255};
    const Color ORANGE_900 = {124, 45, 18, 255};
    const Color ORANGE_950 = {67, 20, 7, 255};

    // Amber
    const Color AMBER_50 = {255, 251, 235, 255};
    const Color AMBER_100 = {254, 243, 199, 255};
    const Color AMBER_200 = {253, 230, 138, 255};
    const Color AMBER_300 = {252, 211, 77, 255};
    const Color AMBER_400 = {251, 191, 36, 255};
    const Color AMBER_500 = {245, 158, 11, 255};
    const Color AMBER_600 = {217, 119, 6, 255};
    const Color AMBER_700 = {180, 83, 9, 255};
    const Color AMBER_800 = {146, 64, 14, 255};
    const Color AMBER_900 = {120, 53, 15, 255};
    const Color AMBER_950 = {69, 26, 3, 255};

    // Yellow
    const Color YELLOW_50 = {254, 252, 232, 255};
    const Color YELLOW_100 = {254, 249, 195, 255};
    const Color YELLOW_200 = {254, 240, 138, 255};
    const Color YELLOW_300 = {253, 224, 71, 255};
    const Color YELLOW_400 = {250, 204, 21, 255};
    const Color YELLOW_500 = {234, 179, 8, 255};
    const Color YELLOW_600 = {202, 138, 4, 255};
    const Color YELLOW_700 = {161, 98, 7, 255};
    const Color YELLOW_800 = {133, 77, 14, 255};
    const Color YELLOW_900 = {113, 63, 18, 255};
    const Color YELLOW_950 = {66, 32, 6, 255};

    // Lime
    const Color LIME_50 = {247, 254, 231, 255};
    const Color LIME_100 = {236, 252, 203, 255};
    const Color LIME_200 = {217, 249, 157, 255};
    const Color LIME_300 = {190, 242, 100, 255};
    const Color LIME_400 = {163, 230, 53, 255};
    const Color LIME_500 = {132, 204, 22, 255};
    const Color LIME_600 = {101, 163, 13, 255};
    const Color LIME_700 = {77, 124, 15, 255};
    const Color LIME_800 = {63, 98, 18, 255};
    const Color LIME_900 = {54, 83, 20, 255};
    const Color LIME_950 = {26, 46, 5, 255};

    // Green
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

    // Emerald
    const Color EMERALD_50 = {236, 253, 245, 255};
    const Color EMERALD_100 = {209, 250, 229, 255};
    const Color EMERALD_200 = {167, 243, 208, 255};
    const Color EMERALD_300 = {110, 231, 183, 255};
    const Color EMERALD_400 = {52, 211, 153, 255};
    const Color EMERALD_500 = {16, 185, 129, 255};
    const Color EMERALD_600 = {5, 150, 105, 255};
    const Color EMERALD_700 = {4, 120, 87, 255};
    const Color EMERALD_800 = {6, 95, 70, 255};
    const Color EMERALD_900 = {6, 78, 59, 255};
    const Color EMERALD_950 = {2, 44, 34, 255};

    // Teal
    const Color TEAL_50 = {240, 253, 250, 255};
    const Color TEAL_100 = {204, 251, 241, 255};
    const Color TEAL_200 = {153, 246, 228, 255};
    const Color TEAL_300 = {94, 234, 212, 255};
    const Color TEAL_400 = {45, 212, 191, 255};
    const Color TEAL_500 = {20, 184, 166, 255};
    const Color TEAL_600 = {13, 148, 136, 255};
    const Color TEAL_700 = {15, 118, 110, 255};
    const Color TEAL_800 = {17, 94, 89, 255};
    const Color TEAL_900 = {19, 78, 74, 255};
    const Color TEAL_950 = {4, 47, 46, 255};

    // Cyan
    const Color CYAN_50 = {236, 254, 255, 255};
    const Color CYAN_100 = {207, 250, 254, 255};
    const Color CYAN_200 = {165, 243, 252, 255};
    const Color CYAN_300 = {103, 232, 249, 255};
    const Color CYAN_400 = {34, 211, 238, 255};
    const Color CYAN_500 = {6, 182, 212, 255};
    const Color CYAN_600 = {8, 145, 178, 255};
    const Color CYAN_700 = {14, 116, 144, 255};
    const Color CYAN_800 = {21, 94, 117, 255};
    const Color CYAN_900 = {22, 78, 99, 255};
    const Color CYAN_950 = {8, 51, 68, 255};

    // Sky
    const Color SKY_50 = {240, 249, 255, 255};
    const Color SKY_100 = {224, 242, 254, 255};
    const Color SKY_200 = {186, 230, 253, 255};
    const Color SKY_300 = {125, 211, 252, 255};
    const Color SKY_400 = {56, 189, 248, 255};
    const Color SKY_500 = {14, 165, 233, 255};
    const Color SKY_600 = {2, 132, 199, 255};
    const Color SKY_700 = {3, 105, 161, 255};
    const Color SKY_800 = {7, 89, 133, 255};
    const Color SKY_900 = {12, 74, 110, 255};
    const Color SKY_950 = {8, 47, 73, 255};

    // Blue
    const Color BLUE_50 = {239, 246, 255, 255};
    const Color BLUE_100 = {219, 234, 254, 255};
    const Color BLUE_200 = {191, 219, 254, 255};
    const Color BLUE_300 = {147, 197, 253, 255};
    const Color BLUE_400 = {96, 165, 250, 255};
    const Color BLUE_500 = {59, 130, 246, 255};
    const Color BLUE_600 = {37, 99, 235, 255};
    const Color BLUE_700 = {29, 78, 216, 255};
    const Color BLUE_800 = {30, 64, 175, 255};
    const Color BLUE_900 = {30, 58, 138, 255};
    const Color BLUE_950 = {23, 37, 84, 255};

    // Indigo
    const Color INDIGO_50 = {238, 242, 255, 255};
    const Color INDIGO_100 = {224, 231, 255, 255};
    const Color INDIGO_200 = {199, 210, 254, 255};
    const Color INDIGO_300 = {165, 180, 252, 255};
    const Color INDIGO_400 = {129, 140, 248, 255};
    const Color INDIGO_500 = {99, 102, 241, 255};
    const Color INDIGO_600 = {79, 70, 229, 255};
    const Color INDIGO_700 = {67, 56, 202, 255};
    const Color INDIGO_800 = {55, 48, 163, 255};
    const Color INDIGO_900 = {49, 46, 129, 255};
    const Color INDIGO_950 = {30, 27, 75, 255};

    // Violet
    const Color VIOLET_50 = {245, 243, 255, 255};
    const Color VIOLET_100 = {237, 233, 254, 255};
    const Color VIOLET_200 = {221, 214, 254, 255};
    const Color VIOLET_300 = {196, 181, 253, 255};
    const Color VIOLET_400 = {167, 139, 250, 255};
    const Color VIOLET_500 = {139, 92, 246, 255};
    const Color VIOLET_600 = {124, 58, 237, 255};
    const Color VIOLET_700 = {109, 40, 217, 255};
    const Color VIOLET_800 = {91, 33, 182, 255};
    const Color VIOLET_900 = {76, 29, 149, 255};
    const Color VIOLET_950 = {46, 16, 101, 255};

    // Purple
    const Color PURPLE_50 = {250, 245, 255, 255};
    const Color PURPLE_100 = {243, 232, 255, 255};
    const Color PURPLE_200 = {233, 213, 255, 255};
    const Color PURPLE_300 = {216, 180, 254, 255};
    const Color PURPLE_400 = {192, 132, 252, 255};
    const Color PURPLE_500 = {168, 85, 247, 255};
    const Color PURPLE_600 = {147, 51, 234, 255};
    const Color PURPLE_700 = {126, 34, 206, 255};
    const Color PURPLE_800 = {107, 33, 168, 255};
    const Color PURPLE_900 = {88, 28, 135, 255};
    const Color PURPLE_950 = {59, 7, 100, 255};

    // Fuchsia
    const Color FUCHSIA_50 = {253, 244, 255, 255};
    const Color FUCHSIA_100 = {250, 232, 255, 255};
    const Color FUCHSIA_200 = {245, 208, 254, 255};
    const Color FUCHSIA_300 = {240, 171, 252, 255};
    const Color FUCHSIA_400 = {232, 121, 249, 255};
    const Color FUCHSIA_500 = {217, 70, 239, 255};
    const Color FUCHSIA_600 = {192, 38, 211, 255};
    const Color FUCHSIA_700 = {162, 28, 175, 255};
    const Color FUCHSIA_800 = {134, 25, 143, 255};
    const Color FUCHSIA_900 = {112, 26, 117, 255};
    const Color FUCHSIA_950 = {74, 4, 78, 255};

    // Pink
    const Color PINK_50 = {253, 242, 248, 255};
    const Color PINK_100 = {252, 231, 243, 255};
    const Color PINK_200 = {251, 207, 232, 255};
    const Color PINK_300 = {249, 168, 212, 255};
    const Color PINK_400 = {244, 114, 182, 255};
    const Color PINK_500 = {236, 72, 153, 255};
    const Color PINK_600 = {219, 39, 119, 255};
    const Color PINK_700 = {190, 24, 93, 255};
    const Color PINK_800 = {157, 23, 77, 255};
    const Color PINK_900 = {131, 24, 67, 255};
    const Color PINK_950 = {80, 7, 36, 255};

    // Rose
    const Color ROSE_50 = {255, 241, 242, 255};
    const Color ROSE_100 = {255, 228, 230, 255};
    const Color ROSE_200 = {254, 205, 211, 255};
    const Color ROSE_300 = {253, 164, 175, 255};
    const Color ROSE_400 = {251, 113, 133, 255};
    const Color ROSE_500 = {244, 63, 94, 255};
    const Color ROSE_600 = {225, 29, 72, 255};
    const Color ROSE_700 = {190, 18, 60, 255};
    const Color ROSE_800 = {159, 18, 57, 255};
    const Color ROSE_900 = {136, 19, 55, 255};
    const Color ROSE_950 = {76, 5, 25, 255};

    // Slate
    const Color SLATE_50 = {248, 250, 252, 255};
    const Color SLATE_100 = {241, 245, 249, 255};
    const Color SLATE_200 = {226, 232, 240, 255};
    const Color SLATE_300 = {203, 213, 225, 255};
    const Color SLATE_400 = {148, 163, 184, 255};
    const Color SLATE_500 = {100, 116, 139, 255};
    const Color SLATE_600 = {71, 85, 105, 255};
    const Color SLATE_700 = {51, 65, 85, 255};
    const Color SLATE_800 = {30, 41, 59, 255};
    const Color SLATE_900 = {15, 23, 42, 255};
    const Color SLATE_950 = {2, 6, 23, 255};

    // Gray
    const Color GRAY_50 = {249, 250, 251, 255};
    const Color GRAY_100 = {243, 244, 246, 255};
    const Color GRAY_200 = {229, 231, 235, 255};
    const Color GRAY_300 = {209, 213, 219, 255};
    const Color GRAY_400 = {156, 163, 175, 255};
    const Color GRAY_500 = {107, 114, 128, 255};
    const Color GRAY_600 = {75, 85, 99, 255};
    const Color GRAY_700 = {55, 65, 81, 255};
    const Color GRAY_800 = {31, 41, 55, 255};
    const Color GRAY_900 = {17, 24, 39, 255};
    const Color GRAY_950 = {3, 7, 18, 255};

    // Zinc
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

    // Neutral
    const Color NEUTRAL_50 = {250, 250, 250, 255};
    const Color NEUTRAL_100 = {245, 245, 245, 255};
    const Color NEUTRAL_200 = {229, 229, 229, 255};
    const Color NEUTRAL_300 = {212, 212, 212, 255};
    const Color NEUTRAL_400 = {163, 163, 163, 255};
    const Color NEUTRAL_500 = {115, 115, 115, 255};
    const Color NEUTRAL_600 = {82, 82, 82, 255};
    const Color NEUTRAL_700 = {64, 64, 64, 255};
    const Color NEUTRAL_800 = {38, 38, 38, 255};
    const Color NEUTRAL_900 = {23, 23, 23, 255};
    const Color NEUTRAL_950 = {10, 10, 10, 255};

    // Stone
    const Color STONE_50 = {250, 250, 249, 255};
    const Color STONE_100 = {245, 245, 244, 255};
    const Color STONE_200 = {231, 229, 228, 255};
    const Color STONE_300 = {214, 211, 209, 255};
    const Color STONE_400 = {168, 162, 158, 255};
    const Color STONE_500 = {120, 113, 108, 255};
    const Color STONE_600 = {87, 83, 78, 255};
    const Color STONE_700 = {68, 64, 60, 255};
    const Color STONE_800 = {41, 37, 36, 255};
    const Color STONE_900 = {28, 25, 23, 255};
    const Color STONE_950 = {12, 10, 9, 255};

    // Black & White
    const Color BLACK = {0, 0, 0, 255};
    const Color WHITE = {255, 255, 255, 255};
} // namespace OpenWars::Colors