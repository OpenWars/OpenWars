#include "input.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>

void OpenWars::IO::Input::Handler::poll() {
    state.scrollY = 0.0f;

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_EVENT_QUIT:
            closeRequested = true;
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            state.scrollY = static_cast<float>(event.wheel.y);
            break;
        }
    }

    const bool* keys = SDL_GetKeyboardState(nullptr);
    auto& d = state.down;

    d.W = keys[SDL_SCANCODE_W];
    d.A = keys[SDL_SCANCODE_A];
    d.S = keys[SDL_SCANCODE_S];
    d.D = keys[SDL_SCANCODE_D];
    d.arrowUp = keys[SDL_SCANCODE_UP];
    d.arrowDown = keys[SDL_SCANCODE_DOWN];
    d.arrowLeft = keys[SDL_SCANCODE_LEFT];
    d.arrowRight = keys[SDL_SCANCODE_RIGHT];
    d.enter = keys[SDL_SCANCODE_RETURN];

    float mx, my;
    uint32_t mouseButtons = SDL_GetMouseState(&mx, &my);
    state.mousePos = {mx, my};

    bool leftDown = (mouseButtons & SDL_BUTTON_LMASK) != 0;
    bool rightDown = (mouseButtons & SDL_BUTTON_RMASK) != 0;
    d.leftClick = leftDown;
    d.rightClick = rightDown;

    const auto& w = prevDown;
    auto& p = state.pressed;

    p.W = d.W && !w.W;
    p.A = d.A && !w.A;
    p.S = d.S && !w.S;
    p.D = d.D && !w.D;
    p.arrowUp = d.arrowUp && !w.arrowUp;
    p.arrowDown = d.arrowDown && !w.arrowDown;
    p.arrowLeft = d.arrowLeft && !w.arrowLeft;
    p.arrowRight = d.arrowRight && !w.arrowRight;
    p.enter = d.enter && !w.enter;
    p.leftClick = leftDown && !w.leftClick;
    p.rightClick = rightDown && !w.rightClick;

    prevDown = d;
}