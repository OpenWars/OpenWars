#include "theme.hpp"
#include "../utils/drawing.hpp"

namespace OpenWars::UI::Theme {
    DrawPanelFn drawPanel = Utils::Drawing::drawParallelogram;
    DrawPanelOutlineFn drawPanelOutline =
        Utils::Drawing::drawParallelogramOutline;
} // namespace OpenWars::UI::Theme