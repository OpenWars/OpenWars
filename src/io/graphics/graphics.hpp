#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace OpenWars::IO::Graphics {
    struct Color {
        uint8_t r, g, b, a;

        Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
            : r(r)
            , g(g)
            , b(b)
            , a(a) {
        }
    };

    struct Vector2 {
        float x, y;

        Vector2(float x = 0, float y = 0)
            : x(x)
            , y(y) {
        }
    };

    struct Vector3 {
        float x, y, z;

        Vector3(float x = 0, float y = 0, float z = 0)
            : x(x)
            , y(y)
            , z(z) {
        }
    };

    struct Rectangle {
        float x, y, width, height;

        Rectangle(float x = 0, float y = 0, float w = 0, float h = 0)
            : x(x)
            , y(y)
            , width(w)
            , height(h) {
        }
    };

    struct Camera3D {
        Vector3 position;
        Vector3 target;
        Vector3 up;
        float fov;
        int projection;
    };

    // Core functions
    void init(int vsync, bool multisampling);
    void displayDebug(bool debug, bool fps);
    void exit();
    bool shouldClose();
    void beginFrame();
    void beginAttached();
    void endAttached();
    Camera3D getCamera();
    void swapBuffers();

    // Drawing functions
    void clearBackground(Color color);
    void drawRectangle(int x, int y, int width, int height, Color color);
    void drawRectangleRec(Rectangle rec, Color color);
    void drawText(const char* text, int x, int y, int fontSize, Color color);
    void drawFPS(int x, int y);
    void drawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
    void drawLineEx(Vector2 start, Vector2 end, float thickness, Color color);

    // Utility functions
    int measureText(const char* text, int fontSize);
    Color colorAlpha(Color color, float alpha);
    Color colorBrightness(Color color, float factor);
    bool checkCollisionPointRec(Vector2 point, Rectangle rec);
    bool
    checkCollisionPointTriangle(Vector2 p, Vector2 v1, Vector2 v2, Vector2 v3);

    // Window functions
    int getScreenWidth();
    int getScreenHeight();
    int getMonitorRefreshRate(int monitor);
    int getCurrentMonitor();

    // Time
    float getFrameTime();
    double getTime();

    // Internal access
    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();
    TTF_Font* getFont(int size);
} // namespace OpenWars::IO::Graphics