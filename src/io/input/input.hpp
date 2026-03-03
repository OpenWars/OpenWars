#pragma once

#include "../../core/vector.hpp"
#include <SDL3/SDL.h>

namespace OpenWars::IO {
    namespace Events {
        class Manager;
    }
} // namespace OpenWars::IO

namespace OpenWars::IO::Input {
    struct InputState {

        struct {
            bool W = false;
            bool A = false;
            bool S = false;
            bool D = false;
            bool arrowLeft = false;
            bool arrowRight = false;
            bool arrowUp = false;
            bool arrowDown = false;
            bool leftClick = false;
            bool rightClick = false;
            bool enter = false;
        } down;

        struct {
            bool W = false;
            bool A = false;
            bool S = false;
            bool D = false;
            bool arrowLeft = false;
            bool arrowRight = false;
            bool arrowUp = false;
            bool arrowDown = false;
            bool leftClick = false;
            bool rightClick = false;
            bool enter = false;
        } wasDown;

        struct {
            bool W = false;
            bool A = false;
            bool S = false;
            bool D = false;
            bool arrowLeft = false;
            bool arrowRight = false;
            bool arrowUp = false;
            bool arrowDown = false;
            bool leftClick = false;
            bool rightClick = false;
            bool enter = false;
        } pressed;

        Vector2 mousePos = {0, 0};
        float scrollY =
            0.0f; // Vertical scroll delta (positive = up, negative = down)
    };

    class Handler {
        InputState state;

      public:
        Handler() {
        }

        void poll(const Events::Manager& eventManager);

        const InputState& getState() const {
            return state;
        }
    };
} // namespace OpenWars::IO::Input