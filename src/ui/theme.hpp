#pragma once

#include "colors.hpp"
#include "../io/graphics/graphics.hpp"

namespace OpenWars::UI::Theme {
    // Spacing
    const int SKEW = 48;
    const float MARGIN = 12.f;

    // Colors
    const IO::Graphics::Color PRIMARY = Colors::GREEN_950;
    const IO::Graphics::Color PRIMARY_FOREGROUND = Colors::GREEN_100;

    const IO::Graphics::Color SECONDARY = Colors::ZINC_700;
    const IO::Graphics::Color SECONDARY_FOREGROUND = Colors::ZINC_300;
} // namespace OpenWars::UI::Theme