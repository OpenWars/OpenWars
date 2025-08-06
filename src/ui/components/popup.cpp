#include "components.hpp"
#include "../../utils/drawing.hpp"
#include <raylib.h>

void OpenWars::UI::PopupComponent::setVisible(bool v) {
    visible = v;
}
bool OpenWars::UI::PopupComponent::isVisible() {
    return visible;
}

void OpenWars::UI::PopupComponent::render() {
    if (!visible) return;

    raylib::Vector2 centerPos = {
        (raylib::GetScreenWidth() - width - 48) / 2.0f,
        (raylib::GetScreenHeight() + height) / 2.0f
    };

    Utils::Drawing::drawParallelogram(centerPos, width, height, 48, raylib::WHITE);
}

bool OpenWars::UI::PopupComponent::handleInput() {
    /*if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON) &&
        Utils::Drawing::pointInQuad(raylib::GetMousePosition(), ctl, ctr, cbr, cbl)) {
        visible = false;
        return true;
    }*/
    return false;
}