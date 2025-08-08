#include "math.hpp"

float OpenWars::Utils::Math::calculateButtonAreaHeight(std::vector<OpenWars::UI::ButtonComponent> buttons) {
    if (buttons.empty()) {
        return 0.0f;
    }
    
    const float BUTTON_SPACING = 8.0f;
    const float BOTTOM_MARGIN = 10.0f;
    const float TOP_MARGIN = 10.0f;
    
    float buttonHeight = buttons[0].height;
    
    return TOP_MARGIN + buttonHeight + BOTTOM_MARGIN;
}