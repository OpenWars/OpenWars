#pragma once

#include "colors.hpp"

namespace raylib {
    #include "raylib.h"
}

namespace OpenWars {
    namespace UI {
        namespace Theme {
            // Spacing
            const int SKEW = 48;
            const float MARGIN = 12.f;

            // Colors
            const raylib::Color PRIMARY = Colors::GREEN_950;
            const raylib::Color PRIMARY_FOREGROUND = Colors::GREEN_100;
        }
    }
}