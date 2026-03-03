#pragma once

#include <SDL3/SDL.h>

namespace OpenWars::IO::Events {
    class Manager {
      public:
        Manager() = default;

        void poll();

        bool shouldClose() const {
            return shouldCloseFlag;
        }

        float getScrollY() const {
            return scrollY;
        }

      private:
        bool shouldCloseFlag = false;
        float scrollY = 0.0f;
    };
} // namespace OpenWars::IO::Events
