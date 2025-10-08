#include "shapes.hpp"
#include "../../io/graphics/graphics.hpp"
#include <SDL3/SDL_pixels.h>
#include <cmath>

void OpenWars::Drawing::drawTriangle(
    Vector2 v1,
    Vector2 v2,
    Vector2 v3,
    Color color
) {
    SDL_FColor fcolor = {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    };

    SDL_Vertex vertices[3] = {
        {{v1.x, v1.y}, fcolor, {0, 0}},
        {{v2.x, v2.y}, fcolor, {0, 0}},
        {{v3.x, v3.y}, fcolor, {0, 0}}
    };

    SDL_RenderGeometry(
        IO::Graphics::getRenderer(),
        nullptr,
        vertices,
        3,
        nullptr,
        0
    );
}

void OpenWars::Drawing::drawLineEx(
    Vector2 start,
    Vector2 end,
    float thickness,
    Color color
) {
    SDL_SetRenderDrawColor(
        IO::Graphics::getRenderer(),
        color.r,
        color.g,
        color.b,
        color.a
    );

    if(thickness <= 1.0f) {
        SDL_RenderLine(
            IO::Graphics::getRenderer(),
            start.x,
            start.y,
            end.x,
            end.y
        );
    } else {
        float angle = atan2f(end.y - start.y, end.x - start.x);
        float halfThick = thickness / 2.0f;

        float perpX = -sinf(angle) * halfThick;
        float perpY = cosf(angle) * halfThick;

        SDL_FColor fcolor = {
            color.r / 255.0f,
            color.g / 255.0f,
            color.b / 255.0f,
            color.a / 255.0f
        };

        SDL_Vertex vertices[4] = {
            {{start.x + perpX, start.y + perpY}, fcolor, {0, 0}},
            {{start.x - perpX, start.y - perpY}, fcolor, {0, 0}},
            {{end.x - perpX, end.y - perpY}, fcolor, {0, 0}},
            {{end.x + perpX, end.y + perpY}, fcolor, {0, 0}}
        };

        int indices[6] = {0, 1, 2, 0, 2, 3};
        SDL_RenderGeometry(
            IO::Graphics::getRenderer(),
            nullptr,
            vertices,
            4,
            indices,
            6
        );
    }
}

void OpenWars::Drawing::drawRectangle(
    int x,
    int y,
    int width,
    int height,
    Color color
) {
    SDL_FRect rect = {(float)x, (float)y, (float)width, (float)height};
    SDL_SetRenderDrawColor(
        IO::Graphics::getRenderer(),
        color.r,
        color.g,
        color.b,
        color.a
    );
    SDL_RenderFillRect(IO::Graphics::getRenderer(), &rect);
}

void OpenWars::Drawing::drawRectangleRec(Drawing::Rectangle rec, Color color) {
    drawRectangle(
        (int)rec.x,
        (int)rec.y,
        (int)rec.width,
        (int)rec.height,
        color
    );
}