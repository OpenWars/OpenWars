#include "components.hpp"
#include "../../utils/drawing.hpp"
#include "../colors.hpp"

void OpenWars::UI::PopupComponent::addButton(ButtonComponent btn){
    buttons.push_back(btn);
};

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
        (raylib::GetScreenWidth() - width - SKEW) / 2.0f,
        (raylib::GetScreenHeight() + height) / 2.0f
    };

    Utils::Drawing::drawParallelogram(centerPos, width, height, SKEW, Colors::ZINC_100);

    // Draw title
    int titleSize = 24;
    float textAreaWidth = width - (MARGIN * 2);
    int textWidth = raylib::MeasureText(title.c_str(), titleSize);

    float centerX = centerPos.x + (width + SKEW) / 2.0f;
    float topY = centerPos.y - height + MARGIN;
    
    raylib::Vector2 titlePos = {
        centerX - textWidth / 2.0f,
        centerPos.y - height - titleSize - MARGIN  // 12px margin
    };
    
    raylib::DrawText(title.c_str(), (int)titlePos.x, (int)titlePos.y, titleSize, Colors::ZINC_300);
    
    Utils::Drawing::drawTextWrapped(message.c_str(), (int)(centerX - textAreaWidth/2) + SKEW, (int)topY, 
                       (int)textAreaWidth, 16, Colors::ZINC_950);

    // Buttons
    if (buttons.empty()){
        buttons.push_back(
            ButtonComponent("I am too cool!", {0,0}, this, 0)
        );
    }

    Utils::Drawing::renderButtons(centerPos, buttons, width, height);
    
}

void OpenWars::UI::PopupComponent::handleButtonInput(int id){

};

bool OpenWars::UI::PopupComponent::handleInput() {
    /*if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON) &&
        Utils::Drawing::pointInQuad(raylib::GetMousePosition(), ctl, ctr, cbr, cbl)) {
        visible = false;
        return true;
    }*/
    return false;
}