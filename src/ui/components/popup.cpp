#include "components.hpp"

void OpenWars::UI::PopupComponent::setVisible(bool v) {
    visible = v;
}
bool OpenWars::UI::PopupComponent::isVisible() {
    return visible;
}

void OpenWars::UI::PopupComponent::render() {
    if (!visible) return;

    raylib::DrawRectangleRec(bounds, raylib::DARKGRAY);
    raylib::DrawRectangleLinesEx(bounds, 2, raylib::BLACK);

    int titleY = bounds.y + 10;
    int msgY = bounds.y + 40;

    raylib::DrawText(title.c_str(), bounds.x + 10, titleY, 20, raylib::WHITE);
    raylib::DrawText(message.c_str(), bounds.x + 10, msgY, 18, raylib::LIGHTGRAY);
    raylib::DrawText("[Press ENTER]", bounds.x + 10, bounds.y + bounds.height - 30, 18, raylib::YELLOW);
}

void OpenWars::UI::PopupComponent::handleInput()  {
    // if (event == noc) visible = false;
}