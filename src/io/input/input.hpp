#pragma once

#include "../../core/vector.hpp"
#include <SDL3/SDL.h>

namespace OpenWars::IO::Input {
    struct InputState {
        struct Keys {
            bool W = false, A = false, S = false, D = false;
            bool arrowLeft = false, arrowRight = false;
            bool arrowUp = false, arrowDown = false;
            bool leftClick = false, rightClick = false;
            bool enter = false;
        };

        Keys down;
        Keys pressed;
        Vector2 mousePos = {0, 0};
        float scrollY = 0.0f;
    };

    class Handler {
        InputState state;
        InputState::Keys prevDown;
        bool closeRequested = false;

        public:
        Handler() = default;

        void poll();

        bool shouldClose() const {
            return closeRequested;
        }

        const InputState& getState() const {
            return state;
        }
    };
} // namespace OpenWars::IO::Input