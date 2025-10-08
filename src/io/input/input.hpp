#pragma once

#include "../../core/vector.hpp"
#include <SDL3/SDL.h>

namespace OpenWars::IO::Input {
    struct InputState {
        bool W = false;
        bool A = false;
        bool S = false;
        bool D = false;

        Vector2 mousePos = {0, 0};
        bool pressingLeft = false;
        bool pressingRight = false;
    };

    class Handler {
        InputState state;

      public:
        Handler() {
        }

        void poll();

        const InputState& getState() const {
            return state;
        }
    };
} // namespace OpenWars::IO::Input