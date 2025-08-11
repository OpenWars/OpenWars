#include "drawing.hpp"
#include <cstddef>
void OpenWars::Utils::Drawing::drawParallelogram(raylib::Vector2 position,
                                                 float width, float height,
                                                 float skew,
                                                 raylib::Color color) {
    raylib::Vector2 v1 = position;
    raylib::Vector2 v2 = {position.x + width, position.y};
    raylib::Vector2 v3 = {position.x + width + skew, position.y - height};
    raylib::Vector2 v4 = {position.x + skew, position.y - height};

    raylib::DrawTriangle(v1, v2, v3, color);
    raylib::DrawTriangle(v1, v3, v4, color);
}

bool OpenWars::Utils::Drawing::pointInQuad(raylib::Vector2 p, raylib::Vector2 a,
                                           raylib::Vector2 b, raylib::Vector2 c,
                                           raylib::Vector2 d) {
    return raylib::CheckCollisionPointTriangle(p, a, b, c) ||
           raylib::CheckCollisionPointTriangle(p, c, d, a);
}

void OpenWars::Utils::Drawing::drawTextWrapped(const char *text, int x, int y,
                                               int maxWidth, int fontSize,
                                               raylib::Color color) {
    if (text == NULL)
        return;

    const char *ptr = text;
    int currentY = y;
    char line[256];
    int lineIndex = 0;

    while (*ptr != '\0') {
        if (*ptr == '\n') {
            line[lineIndex] = '\0';
            raylib::DrawText(line, x, currentY, fontSize, color);
            currentY += fontSize + 2;
            lineIndex = 0;
            ptr++;
            continue;
        }

        line[lineIndex] = *ptr;
        line[lineIndex + 1] = '\0';

        if (raylib::MeasureText(line, fontSize) > maxWidth && lineIndex > 0) {
            int lastSpace = lineIndex - 1;
            while (lastSpace > 0 && line[lastSpace] != ' ') {
                lastSpace--;
            }

            if (lastSpace > 0) {
                line[lastSpace] = '\0';
                raylib::DrawText(line, x, currentY, fontSize, color);
                currentY += fontSize + 2;

                ptr -= (lineIndex - lastSpace - 1);
                lineIndex = 0;
            } else {
                line[lineIndex] = '\0';
                raylib::DrawText(line, x, currentY, fontSize, color);
                currentY += fontSize + 2;
                lineIndex = 0;
            }
        } else {
            lineIndex++;
            ptr++;
        }
    }

    if (lineIndex > 0) {
        line[lineIndex] = '\0';
        raylib::DrawText(line, x, currentY, fontSize, color);
    }
}

void OpenWars::Utils::Drawing::renderButtons(
    raylib::Vector2 parallelogramPos,
    std::vector<OpenWars::UI::ButtonComponent> buttons, float width,
    float buttonAreaHeight) {

    const float BUTTON_SPACING = 8.0f;
    const float ROW_SPACING = 4.0f;
    const float SIDE_MARGIN = 10.0f;

    float availableWidth = width - (SIDE_MARGIN * 2);
    float currentX = parallelogramPos.x + SIDE_MARGIN;
    float currentY = parallelogramPos.y - buttonAreaHeight + 10.0f;

    for (auto &button : buttons) {
        if (currentX + button.width >
            parallelogramPos.x + SIDE_MARGIN + availableWidth) {
            currentX = parallelogramPos.x + SIDE_MARGIN;
            currentY += button.height + ROW_SPACING;
        }

        button.position = raylib::Vector2{currentX, currentY};
        button.render();

        currentX += button.width + BUTTON_SPACING;
    }
}