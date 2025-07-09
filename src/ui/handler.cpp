#include "ui.hpp"

void OpenWars::UI::Handler::addComponent(std::unique_ptr<Component> c) {
    components.push_back(std::move(c));
}

void OpenWars::UI::Handler::handleInput() {
    for (const auto& c : components) {
        c->handleInput();
    }
}

void OpenWars::UI::Handler::renderOverlay(){
    for (const auto& c : components) {
        c->render();
    }
}