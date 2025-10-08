#pragma once

#include "../colors.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <map>

namespace OpenWars::Drawing {
    static std::map<int, TTF_Font*> fonts;

    TTF_Font* getFont(int size);
    void drawText(const char* text, int x, int y, int fontSize, Color color);
    int measureText(const char* text, int fontSize);
    void drawFPS(int x, int y, int currentFPS);
} // namespace OpenWars::Drawing