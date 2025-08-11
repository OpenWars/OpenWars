#include "ui.hpp"

void OpenWars::UI::Handler::addComponent(std::unique_ptr<Component> c) {
    components.push_back(std::move(c));
}

bool OpenWars::UI::Handler::handleInput(const IO::Input::InputState &state) {
    bool consumed = false;
    for (auto &comp : components) {
        if (comp->handleInput(state)) {
            consumed = true;
        }
    }
    return consumed;
}

void OpenWars::UI::Handler::renderOverlay() {
    for (const auto &c : components) {
        c->render();
    }
}