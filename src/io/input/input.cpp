#include "input.hpp"
#include <SDL3/SDL_scancode.h>

void OpenWars::IO::Input::Handler::poll() {
    const bool* keystate = SDL_GetKeyboardState(nullptr);

    state.W = keystate[SDL_SCANCODE_W];
    state.A = keystate[SDL_SCANCODE_A];
    state.S = keystate[SDL_SCANCODE_S];
    state.D = keystate[SDL_SCANCODE_D];
    state.ArrowUp = keystate[SDL_SCANCODE_UP];
    state.ArrowDown = keystate[SDL_SCANCODE_DOWN];
    state.ArrowLeft = keystate[SDL_SCANCODE_LEFT];
    state.ArrowRight = keystate[SDL_SCANCODE_RIGHT];

    float mouseX, mouseY;
    uint32_t mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    state.mousePos.x = mouseX;
    state.mousePos.y = mouseY;

    bool down_left = (mouseState & SDL_BUTTON_LMASK) != 0;
    bool down_right = (mouseState & SDL_BUTTON_RMASK) != 0;

    state.pressingLeft = down_left && down_left != state.downLeft;
    state.pressingRight = down_right && down_right != state.downRight;

    state.downLeft = down_left;
    state.downRight = down_right;
}