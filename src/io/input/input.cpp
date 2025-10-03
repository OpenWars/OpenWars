#include "input.hpp"

void OpenWars::IO::Input::Handler::poll() {
    const bool* keystate = SDL_GetKeyboardState(nullptr);

    state.W = keystate[SDL_SCANCODE_W];
    state.A = keystate[SDL_SCANCODE_A];
    state.S = keystate[SDL_SCANCODE_S];
    state.D = keystate[SDL_SCANCODE_D];

    float mouseX, mouseY;
    uint32_t mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    state.mousePos.x = mouseX;
    state.mousePos.y = mouseY;
    state.pressingLeft = (mouseState & SDL_BUTTON_LMASK) != 0;
    state.pressingRight = (mouseState & SDL_BUTTON_RMASK) != 0;
}