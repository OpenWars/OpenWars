#include "../../utils/drawing.hpp"
#include "../../utils/math.hpp"
#include "../colors.hpp"
#include "components.hpp"

void OpenWars::UI::PopupComponent::addButton(ButtonComponent btn) {
    buttons.push_back(btn);
};

void OpenWars::UI::PopupComponent::setVisible(bool v) { visible = v; }
bool OpenWars::UI::PopupComponent::isVisible() { return visible; }

void OpenWars::UI::PopupComponent::render() {
    if (!visible)
        return;

    // Layout
    raylib::Vector2 centerPos = {(raylib::GetScreenWidth() - width - SKEW) /
                                     2.0f,
                                 (raylib::GetScreenHeight() + height) / 2.0f};

    // Layout - Shadow
    raylib::Vector2 shadowOffset = {centerPos.x + 3, centerPos.y + 3};
    Utils::Drawing::drawParallelogram(shadowOffset, width, height, SKEW,
                                      Colors::ZINC_600);

    // Layout - Background
    Utils::Drawing::drawParallelogram(centerPos, width, height, SKEW,
                                      Colors::ZINC_800);

    // Layout - Outline
    Utils::Drawing::drawParallelogramOutline(centerPos, width, height, SKEW,
                                             Colors::ZINC_600, 2);

    // Title
    int titleSize = 22;
    float textAreaWidth = width - (MARGIN * 2);
    int textWidth = raylib::MeasureText(title.c_str(), titleSize);

    float centerX = centerPos.x + (width + SKEW) / 2.0f;
    float topY = centerPos.y - height + MARGIN + 32;

    raylib::Vector2 titlePos = {centerX - textWidth / 2.0f,
                                centerPos.y - height + MARGIN - 6};

    // Title - Background
    float titleBarY = centerPos.y - height + 32;
    float heightFromTop = 32;
    float skewOffsetAtTitleBar = (heightFromTop / height) * SKEW;
    raylib::Vector2 titleBarPos = {centerPos.x + SKEW - skewOffsetAtTitleBar,
                                   titleBarY};
    Utils::Drawing::drawParallelogram(titleBarPos, width, 32,
                                      skewOffsetAtTitleBar, Colors::ZINC_600);
    
    raylib::DrawText(title.c_str(), (int)titlePos.x, (int)titlePos.y,
       titleSize, Colors::ZINC_200);

    Utils::Drawing::drawTextWrapped(
            message.c_str(), (int)(centerX - textAreaWidth / 2) + SKEW,
       (int)topY, (int)textAreaWidth, 16, Colors::ZINC_100);

    // Buttons
    if (buttons.empty()) {
        buttons.push_back(ButtonComponent("I am too cool!", {0, 0}, this, 0));
    }

    Utils::Drawing::renderButtons(
        centerPos, buttons, width,
        Utils::Math::calculateButtonAreaHeight(buttons));
}

void OpenWars::UI::PopupComponent::handleButtonInput(int id) {

};

bool OpenWars::UI::PopupComponent::handleInput(
    const IO::Input::InputState &state) {
    return false;
}