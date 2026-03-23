#pragma once
#include "../core/colors.hpp"
#include "../core/vector.hpp"

namespace OpenWars::UI::Theme {
    inline int SKEW = 48;
    inline float MARGIN = 12.f;

    inline Color PRIMARY = Colors::GREEN_950;
    inline Color PRIMARY_FOREGROUND = Colors::GREEN_100;
    inline Color SECONDARY = Colors::ZINC_700;
    inline Color SECONDARY_FOREGROUND = Colors::ZINC_300;
    inline Color FOCUS = Colors::GREEN_400;

    using DrawPanelFn = void (*)(Vector2, float, float, float, Color);
    using DrawPanelOutlineFn =
        void (*)(Vector2, float, float, float, Color, float);

    extern DrawPanelFn drawPanel;
    extern DrawPanelOutlineFn drawPanelOutline;
} // namespace OpenWars::UI::Theme