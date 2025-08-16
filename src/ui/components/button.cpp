#include "../../utils/drawing.hpp"
#include "components.hpp"
#include <raylib.h>

void OpenWars::UI::ButtonComponent::render() {
    int skew = Theme::SKEW / 3.f;

    raylib::Color bg =
        isHovered ? raylib::ColorBrightness(background, 0.2) : background;
    raylib::Color outline = isHovered
                                ? raylib::ColorBrightness(Colors::ZINC_500, 0.2)
                                : Colors::ZINC_500;
    raylib::Color shadow = isHovered
                               ? raylib::ColorBrightness(Colors::ZINC_600, 0.2)
                               : Colors::ZINC_600;

    raylib::Vector2 shadowPos = {position.x + 2, position.y + 2};
    Utils::Drawing::drawParallelogram(shadowPos, width, height, skew, shadow);

    Utils::Drawing::drawParallelogram(position, width, height, skew, bg);
    Utils::Drawing::drawParallelogramOutline(
        position,
        width,
        height,
        skew,
        outline,
        1.5f
    );

    float centerX = position.x + (width + skew) / 2.0f;
    float centerY = position.y - height / 2.0f;

    int fontSize = 14;
    int textWidth = raylib::MeasureText(label.c_str(), fontSize);
    raylib::Vector2 labelPos = {
        centerX - textWidth / 2.0f,
        centerY - fontSize / 2.0f
    };
    raylib::DrawText(
        label.c_str(),
        labelPos.x,
        labelPos.y,
        fontSize,
        foreground
    );
}

bool OpenWars::UI::ButtonComponent::handleInput(
    const IO::Input::InputState& state
) {
    float skew = Theme::SKEW / 3.f;

    raylib::Rectangle bounds =
        {position.x, position.y - height, width + skew, height};

    isHovered = raylib::CheckCollisionPointRec(state.mousePos, bounds);

    if(isHovered && state.pressingLeft) {
        this->parent->handleButtonInput(this->id);
        return true;
    }

    return false;
}