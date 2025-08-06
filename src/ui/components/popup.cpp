#include "components.hpp"
#include "../../utils/drawing.hpp"
#include "../colors.hpp"
#include <raylib.h>

void OpenWars::UI::PopupComponent::setVisible(bool v) {
    visible = v;
}
bool OpenWars::UI::PopupComponent::isVisible() {
    return visible;
}

void OpenWars::UI::PopupComponent::render() {
    if (!visible) return;


    // Draw background
    raylib::Vector2 centerPos = {
        (raylib::GetScreenWidth() - width - 48) / 2.0f,
        (raylib::GetScreenHeight() + height) / 2.0f
    };

    Utils::Drawing::drawParallelogram(centerPos, width, height, 48, Colors::ZINC_100);

    // Calcular posición del texto arriba del paralelogramo
    int textWidth = raylib::MeasureText(title.c_str(), 24);
    float centerX = centerPos.x + (width + 48) / 2.0f;
    
    raylib::Vector2 textPos = {
        centerX - textWidth / 2.0f,
        centerPos.y - height - 24 - 10  // 10 píxeles de separación
    };
    
    raylib::DrawText(title.c_str(), (int)textPos.x, (int)textPos.y, 24, Colors::ZINC_300);
}

bool OpenWars::UI::PopupComponent::handleInput() {
    /*if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON) &&
        Utils::Drawing::pointInQuad(raylib::GetMousePosition(), ctl, ctr, cbr, cbl)) {
        visible = false;
        return true;
    }*/
    return false;
}