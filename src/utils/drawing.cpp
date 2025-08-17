#include "drawing.hpp"
#include <cstddef>

void OpenWars::Utils::Drawing::drawParallelogram(
    raylib::Vector2 position,
    float width,
    float height,
    float skew,
    raylib::Color color
) {
    raylib::Vector2 v1 = position;
    raylib::Vector2 v2 = {position.x + width, position.y};
    raylib::Vector2 v3 = {position.x + width + skew, position.y - height};
    raylib::Vector2 v4 = {position.x + skew, position.y - height};

    raylib::DrawTriangle(v1, v2, v3, color);
    raylib::DrawTriangle(v1, v3, v4, color);
}

void OpenWars::Utils::Drawing::drawParallelogramOutline(
    raylib::Vector2 position,
    float width,
    float height,
    float skew,
    raylib::Color color,
    float thickness
) {
    raylib::Vector2 v1 = position;
    raylib::Vector2 v2 = {position.x + width, position.y};
    raylib::Vector2 v3 = {position.x + width + skew, position.y - height};
    raylib::Vector2 v4 = {position.x + skew, position.y - height};

    DrawLineEx(v1, v2, thickness, color);
    DrawLineEx(v2, v3, thickness, color);
    DrawLineEx(v3, v4, thickness, color);
    DrawLineEx(v4, v1, thickness, color);
}

bool OpenWars::Utils::Drawing::pointInQuad(
    raylib::Vector2 p,
    raylib::Vector2 a,
    raylib::Vector2 b,
    raylib::Vector2 c,
    raylib::Vector2 d
) {
    return raylib::CheckCollisionPointTriangle(p, a, b, c) ||
           raylib::CheckCollisionPointTriangle(p, c, d, a);
}

void OpenWars::Utils::Drawing::drawTextWrapped(
    const std::string& text,
    int x,
    int y,
    int maxWidth,
    int fontSize,
    raylib::Color color
) {
    if(text.empty())
        return;

    size_t pos = 0;
    int currentY = y;
    std::string line;

    while(pos < text.length()) {
        if(text[pos] == '\n') {
            raylib::DrawText(line.c_str(), x, currentY, fontSize, color);
            currentY += fontSize + 2;
            line.clear();
            pos++;
            continue;
        }

        line += text[pos];

        if(raylib::MeasureText(line.c_str(), fontSize) > maxWidth &&
           line.length() > 1) {
            size_t lastSpace = line.find_last_of(' ');

            if(lastSpace != std::string::npos && lastSpace > 0) {
                std::string drawLine = line.substr(0, lastSpace);
                raylib::DrawText(
                    drawLine.c_str(),
                    x,
                    currentY,
                    fontSize,
                    color
                );
                currentY += fontSize + 2;

                pos -= (line.length() - lastSpace - 1);
                line.clear();
            } else {
                line.pop_back();
                raylib::DrawText(line.c_str(), x, currentY, fontSize, color);
                currentY += fontSize + 2;
                pos--;
                line.clear();
            }
        } else {
            pos++;
        }
    }

    if(!line.empty()) {
        raylib::DrawText(line.c_str(), x, currentY, fontSize, color);
    }
}

void OpenWars::Utils::Drawing::renderButtons(
    raylib::Vector2 parallelogramPos,
    std::vector<OpenWars::UI::ButtonComponent*> buttons,
    float width,
    float buttonAreaHeight
) {
    const float BUTTON_SPACING = 8.0f;
    const float ROW_SPACING = 4.0f;
    const float SIDE_MARGIN = 10.0f;

    float availableWidth = width - (SIDE_MARGIN * 2);
    float currentX = parallelogramPos.x + SIDE_MARGIN;
    float currentY = parallelogramPos.y - buttonAreaHeight + 10.0f;

    for(auto& button : buttons) {
        if(button->position.x == 0.f && button->position.y == 0.f) {
            if(currentX + button->width >
               parallelogramPos.x + SIDE_MARGIN + availableWidth) {
                currentX = parallelogramPos.x + SIDE_MARGIN;
                currentY += button->height + ROW_SPACING;
            }

            button->position = raylib::Vector2{currentX, currentY};
            currentX += button->width + BUTTON_SPACING;
        }
        button->render();
    }
}