#include "ui.hpp"

void OpenWars::UI::Handler::addComponent(std::unique_ptr<Component> c) {
    components.push_back(std::move(c));
}

bool OpenWars::UI::Handler::handleInput() {
    if (*components.rbegin()) {
        return components.rbegin()->get()->handleInput();
    }
    return false;
}

void OpenWars::UI::Handler::renderOverlay() {
    for (const auto &c : components) {
        c->render();
    }
}