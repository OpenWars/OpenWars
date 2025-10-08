#include "text.hpp"
#include "../../io/graphics/graphics.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <string>

TTF_Font* OpenWars::Drawing::getFont(int size) {
    if(fonts.find(size) == fonts.end()) {
        fonts[size] = TTF_OpenFont(
            "/usr/share/fonts/liberation/LiberationSans-Regular.ttf",
            size
        );
        /*
        For MacOS?
        if(!fonts[size]) {
            fonts[size] =
                TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", size);
        }*/
    }
    return fonts[size];
}

void OpenWars::Drawing::drawText(
    const char* text,
    int x,
    int y,
    int fontSize,
    Color color
) {
    TTF_Font* font = getFont(fontSize);
    if(!font)
        return;

    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, sdlColor);
    if(!surface)
        return;

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(IO::Graphics::getRenderer(), surface);
    if(texture) {
        SDL_FRect dstRect =
            {(float)x, (float)y, (float)surface->w, (float)surface->h};
        SDL_RenderTexture(
            IO::Graphics::getRenderer(),
            texture,
            nullptr,
            &dstRect
        );
        SDL_DestroyTexture(texture);
    }

    SDL_DestroySurface(surface);
}

void OpenWars::Drawing::drawFPS(int x, int y, int currentFPS) {
    std::string fpsText = std::to_string(currentFPS) + " FPS";
    drawText(fpsText.c_str(), x, y, 20, Color(0, 255, 0, 255));
}

int OpenWars::Drawing::measureText(const char* text, int fontSize) {
    TTF_Font* font = getFont(fontSize);
    if(!font)
        return 0;

    int w = 0;
    TTF_GetStringSize(font, text, 0, &w, nullptr);
    return w;
}