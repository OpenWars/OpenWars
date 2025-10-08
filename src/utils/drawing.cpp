#include "drawing.hpp"
#include "../core/drawing/shapes.hpp"
#include "../core/drawing/collision.hpp"
#include "../core/drawing/text.hpp"
#include <cstddef>

using namespace OpenWars::Drawing;

void OpenWars::Utils::Drawing::drawParallelogram(
    Vector2 position,
    float width,
    float height,
    float skew,
    Color color
) {
    Vector2 v1 = position;
    Vector2 v2 = {position.x + width, position.y};
    Vector2 v3 = {position.x + width + skew, position.y - height};
    Vector2 v4 = {position.x + skew, position.y - height};

    drawTriangle(v1, v2, v3, color);
    drawTriangle(v1, v3, v4, color);
}

void OpenWars::Utils::Drawing::drawParallelogramOutline(
    Vector2 position,
    float width,
    float height,
    float skew,
    Color color,
    float thickness
) {
    Vector2 v1 = position;
    Vector2 v2 = {position.x + width, position.y};
    Vector2 v3 = {position.x + width + skew, position.y - height};
    Vector2 v4 = {position.x + skew, position.y - height};

    drawLineEx(v1, v2, thickness, color);
    drawLineEx(v2, v3, thickness, color);
    drawLineEx(v3, v4, thickness, color);
    drawLineEx(v4, v1, thickness, color);
}

bool OpenWars::Utils::Drawing::pointInQuad(
    Vector2 p,
    Vector2 a,
    Vector2 b,
    Vector2 c,
    Vector2 d
) {
    return checkCollisionPointTriangle(p, a, b, c) ||
           checkCollisionPointTriangle(p, c, d, a);
}

// is this needed?
void OpenWars::Utils::Drawing::drawTextWrapped(
    const std::string& text,
    int x,
    int y,
    int maxWidth,
    int fontSize,
    Color color
) {
    if(text.empty())
        return;

    size_t pos = 0;
    int currentY = y;
    std::string line;

    while(pos < text.length()) {
        if(text[pos] == '\n') {
            drawText(line.c_str(), x, currentY, fontSize, color);
            currentY += fontSize + 2;
            line.clear();
            pos++;
            continue;
        }

        line += text[pos];

        if(measureText(line.c_str(), fontSize) > maxWidth &&
           line.length() > 1) {
            size_t lastSpace = line.find_last_of(' ');

            if(lastSpace != std::string::npos && lastSpace > 0) {
                std::string drawLine = line.substr(0, lastSpace);
                drawText(drawLine.c_str(), x, currentY, fontSize, color);
                currentY += fontSize + 2;

                pos -= (line.length() - lastSpace - 1);
                line.clear();
            } else {
                line.pop_back();
                drawText(line.c_str(), x, currentY, fontSize, color);
                currentY += fontSize + 2;
                pos--;
                line.clear();
            }
        } else {
            pos++;
        }
    }

    if(!line.empty()) {
        drawText(line.c_str(), x, currentY, fontSize, color);
    }
}