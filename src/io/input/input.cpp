#include "input.hpp"
#include <SDL3/SDL_scancode.h>
void OpenWars::IO::Input::Handler::poll() {
    const bool* keystate = SDL_GetKeyboardState(nullptr);

    // --- current down state ---
    state.down.W = keystate[SDL_SCANCODE_W];
    state.down.A = keystate[SDL_SCANCODE_A];
    state.down.S = keystate[SDL_SCANCODE_S];
    state.down.D = keystate[SDL_SCANCODE_D];
    state.down.arrowUp = keystate[SDL_SCANCODE_UP];
    state.down.arrowDown = keystate[SDL_SCANCODE_DOWN];
    state.down.arrowLeft = keystate[SDL_SCANCODE_LEFT];
    state.down.arrowRight = keystate[SDL_SCANCODE_RIGHT];
    state.down.enter = keystate[SDL_SCANCODE_RETURN];

    // --- pressed this frame ---
    state.pressed.W = state.down.W && !state.wasDown.W;
    state.pressed.A = state.down.A && !state.wasDown.A;
    state.pressed.S = state.down.S && !state.wasDown.S;
    state.pressed.D = state.down.D && !state.wasDown.D;
    state.pressed.arrowUp = state.down.arrowUp && !state.wasDown.arrowUp;
    state.pressed.arrowDown = state.down.arrowDown && !state.wasDown.arrowDown;
    state.pressed.arrowLeft = state.down.arrowLeft && !state.wasDown.arrowLeft;
    state.pressed.arrowRight =
        state.down.arrowRight && !state.wasDown.arrowRight;
    state.pressed.enter = state.down.enter && !state.wasDown.enter;

    // --- mouse ---
    float mouseX, mouseY;
    uint32_t mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    state.mousePos.x = mouseX;
    state.mousePos.y = mouseY;

    bool down_left = (mouseState & SDL_BUTTON_LMASK) != 0;
    bool down_right = (mouseState & SDL_BUTTON_RMASK) != 0;

    state.pressed.leftClick = down_left && !state.wasDown.leftClick;
    state.pressed.rightClick = down_right && !state.wasDown.rightClick;

    state.down.leftClick = down_left;
    state.down.rightClick = down_right;

    // --- save down state for next frame ---
    state.wasDown.W = state.down.W;
    state.wasDown.A = state.down.A;
    state.wasDown.S = state.down.S;
    state.wasDown.D = state.down.D;
    state.wasDown.arrowUp = state.down.arrowUp;
    state.wasDown.arrowDown = state.down.arrowDown;
    state.wasDown.arrowLeft = state.down.arrowLeft;
    state.wasDown.arrowRight = state.down.arrowRight;
    state.wasDown.leftClick = state.down.leftClick;
    state.wasDown.rightClick = state.down.rightClick;
    state.wasDown.enter = state.down.enter;
}
