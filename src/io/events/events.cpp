#include "events.hpp"

namespace OpenWars::IO::Events {
    void Manager::poll() {
        scrollY = 0.0f;

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                shouldCloseFlag = true;
            } else if(event.type == SDL_EVENT_MOUSE_WHEEL) {
                scrollY = static_cast<float>(event.wheel.y);
            }
        }
    }
} // namespace OpenWars::IO::Events
