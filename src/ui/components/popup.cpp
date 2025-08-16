#include "../../utils/drawing.hpp"
#include "../../utils/math.hpp"
#include "../colors.hpp"
#include "components.hpp"
void OpenWars::UI::PopupComponent::addButton(ButtonComponent* btn) {
    buttons.push_back(btn);
};

void OpenWars::UI::PopupComponent::setVisible(bool v) {
    visible = v;
}
bool OpenWars::UI::PopupComponent::isVisible() {
    return visible;
}

void OpenWars::UI::PopupComponent::render() {
    if(!visible)
        return;

    // Layout
    raylib::Vector2 centerPos = {
        (raylib::GetScreenWidth() - width - Theme::SKEW) / 2.0f,
        (raylib::GetScreenHeight() + height) / 2.0f
    };

    // Layout - Shadow
    raylib::Vector2 shadowOffset = {centerPos.x + 3, centerPos.y + 3};
    Utils::Drawing::drawParallelogram(
        shadowOffset,
        width,
        height,
        Theme::SKEW,
        Colors::ZINC_600
    );

    // Layout - Background
    Utils::Drawing::drawParallelogram(
        centerPos,
        width,
        height,
        Theme::SKEW,
        Colors::ZINC_800
    );

    // Layout - Outline
    Utils::Drawing::drawParallelogramOutline(
        centerPos,
        width,
        height,
        Theme::SKEW,
        Colors::ZINC_600,
        2
    );

    // Title
    int titleSize = 22;
    float textAreaWidth = width - (Theme::MARGIN * 2);
    int textWidth = raylib::MeasureText(title.c_str(), titleSize);

    float centerX = centerPos.x + (width + Theme::SKEW) / 2.0f;
    float topY = centerPos.y - height + Theme::MARGIN + 32;

    raylib::Vector2 titlePos = {
        centerX - textWidth / 2.0f,
        centerPos.y - height + Theme::MARGIN - 6
    };

    // Title - Background
    float titleBarY = centerPos.y - height + 32;
    float heightFromTop = 32;
    float skewOffsetAtTitleBar = (heightFromTop / height) * Theme::SKEW;
    raylib::Vector2 titleBarPos = {
        centerPos.x + Theme::SKEW - skewOffsetAtTitleBar,
        titleBarY
    };
    Utils::Drawing::drawParallelogram(
        titleBarPos,
        width,
        32,
        skewOffsetAtTitleBar,
        Colors::ZINC_600
    );

    raylib::DrawText(
        title.c_str(),
        (int)titlePos.x,
        (int)titlePos.y,
        titleSize,
        Colors::ZINC_200
    );

    // Content
    Utils::Drawing::drawTextWrapped(
        message,
        (int)(centerX - textAreaWidth / 2) + Theme::SKEW,
        (int)topY,
        (int)textAreaWidth,
        16,
        Colors::ZINC_100
    );

    // Buttons
    if(buttons.empty()) {
        buttons.push_back(
            new ButtonComponent("I am too cool!", {0, 0}, this, 0)
        );
    }

    Utils::Drawing::renderButtons(
        centerPos,
        buttons,
        width,
        Utils::Math::calculateButtonAreaHeight(buttons)
    );
}

void OpenWars::UI::PopupComponent::handleButtonInput(int id) {};

bool OpenWars::UI::PopupComponent::handleInput(
    const IO::Input::InputState& state
) {

    bool consumed = false;
    for(auto button : buttons) {
        if(button->handleInput(state)) {
            consumed = true;
        }
    }
    return consumed;
}