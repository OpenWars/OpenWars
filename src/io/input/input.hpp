#pragma once

namespace raylib {
#include "raylib.h"
}

namespace OpenWars::IO::Input {
    struct InputState {
        bool W = false;
        bool A = false;
        bool S = false;
        bool D = false;

        raylib::Vector2 mousePos = {0, 0};
        bool pressingLeft = false;
        bool pressingRight = false;
    };

    class Handler {
        InputState state;

      public:
        Handler() {
        }

        void poll();
    };
} // namespace OpenWars::IO::Input
