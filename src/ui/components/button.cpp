#include "../../utils/drawing.hpp"
#include "components.hpp"
#include <raylib.h>

void OpenWars::UI::ButtonComponent::render() {
    Utils::Drawing::drawParallelogram(position, width, height, SKEW / 3.f,
                                      background);

    float centerX = position.x + (width + SKEW / 3.f) / 2.0f;
    float centerY = position.y - height / 2.0f;
    int textWidth = raylib::MeasureText(label.c_str(), 12);

    raylib::Vector2 labelPos = {
        centerX - textWidth / 2.0f,
        centerY - 6 // 6 = fontSize/2 para centrado vertical
    };

    raylib::DrawText(label.c_str(), labelPos.x, labelPos.y, 12, foreground);
}

bool OpenWars::UI::ButtonComponent::handleInput(const IO::Input::InputState &state) {
    // parent->handleButtonInput(this->id);
    return false;
}