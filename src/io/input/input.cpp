#include "input.hpp"

void OpenWars::IO::Input::PollHandler::poll() {
    state.W = IsKeyDown(raylib::KEY_W);
    state.A = IsKeyDown(raylib::KEY_A);
    state.S = IsKeyDown(raylib::KEY_S);
    state.D = IsKeyDown(raylib::KEY_D);

    state.mousePos = raylib::GetMousePosition();
    state.pressingLeft = raylib::IsMouseButtonDown(raylib::MOUSE_LEFT_BUTTON);
    state.pressingRight = raylib::IsMouseButtonDown(raylib::MOUSE_RIGHT_BUTTON);
}

void OpenWars::IO::Input::PollHandler::handle() {
    for (auto &handler : boundHandlers) {
        handler->handle(state);
    }
}