#include "../../utils/drawing.hpp"
#include "components.hpp"
#include <raylib.h>

void OpenWars::UI::ButtonComponent::render() {

    int skew = Theme::SKEW / 3.f;

    raylib::Vector2 shadowPos = {position.x + 2, position.y + 2};
    Utils::Drawing::drawParallelogram(shadowPos, width, height, skew,
                                      Colors::ZINC_400);

    Utils::Drawing::drawParallelogram(position, width, height, skew,
                                      background);
    Utils::Drawing::drawParallelogramOutline(position, width, height, skew,
                                             Colors::ZINC_500, 1.5f);

    float centerX = position.x + (width + skew) / 2.0f;
    float centerY = position.y - height / 2.0f;

    int fontSize = 14;
    int textWidth = raylib::MeasureText(label.c_str(), fontSize);
    raylib::Vector2 labelPos = {
        centerX - textWidth / 2.0f,
        centerY - fontSize / 2.0f
    };
    raylib::DrawText(label.c_str(), labelPos.x, labelPos.y, fontSize,
                     foreground);
}

bool OpenWars::UI::ButtonComponent::handleInput(
    const IO::Input::InputState &state) {
    // parent->handleButtonInput(this->id);
    return false;
}